#pragma

//										 openGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//										STB image loader
#define STB_IMAGE_IMPLEMENTATION
//										 standard template library
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
//										 openGL Math libraries
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//										imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
//
#include "CommonValues.h"
//										 project 
#include "Mesh.h"
#include "Shader.h"
#include "GL_Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "SkyBox.h"
#include "Scene.h"
#include "GraphicUI.h"
#include "FrameBuffer.h"
#include "DualFrameBuffer.h"



const float toRadians = 3.14159265 / 180.0;

GL_Window mainWindow;
FrameBuffer framebufferHDR;
DualFrameBuffer dualFramebufferHDR;
Camera camera;

Scene mainScene;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader outlineShader;
Shader framebuffershader;
Shader dualFramebuffershader;

GLuint uniformProjection = 0,
	   uniformModel = 0,
	   uniformView = 0,
	   uniformEyePosition = 0,
	   uniformEyeDirection = 0,
	   uniformSpecularIntensity = 0,
	   uniformSpecularPower = 0,
	   uniformOmniLightPos = 0,
	   uniformFarPlane = 0;

unsigned short int currentShader = 0;

// main shader program hard locations
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";
static const char* olvShader = "Shaders/olfshader.vert";
static const char* olfShader = "Shaders/olfshader.frag";
static const char* outlineVShader = "Shaders/outline.vert";
static const char* outlineFShader = "Shaders/outline.frag";
static const char* FBVShader = "Shaders/framebuffershader.vert";
static const char* FBFShader = "Shaders/framebuffershader.frag";
static const char* DFBVShader = "Shaders/dualFramebuffershader.vert";
static const char* DFBFShader = "Shaders/dualFramebuffershader.frag";

Material shineMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;	
GLfloat now = 0.0f;
GLfloat spin;

GLboolean splitScreenIsOn = false;
GLuint splitScreenType = 0;
GLfloat gamma = 1.0f;
GLfloat bloomThreshold = 1.0f;

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned short int spotLightCount = 0;
unsigned short int pointLightCount = 0;

unsigned int screenQuadVAO, screenQuadVBO;


void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	outlineShader = Shader();
	outlineShader.CreateFromFiles(outlineVShader, outlineFShader);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", 
											"Shaders/directional_shadow_map.frag"
	);
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert",
											"Shaders/omni_shadow_map.geom",
											"Shaders/omni_shadow_map.frag"
	);

	framebuffershader = Shader();
	framebuffershader.CreateFromFiles(FBVShader, FBFShader);

	dualFramebuffershader = Shader();
	dualFramebuffershader.CreateFromFiles(DFBVShader, DFBFShader);
}

void RenderScene() {
	for (auto element = mainScene.objects.begin() ; element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);

		model = glm::rotate(model, spin * toRadians, glm::vec3(0.0, 1.0, 0.0));
		//model = glm::translate(model, glm::vec3(2.0, 0.85, 3.0));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower);

		(*element).RenderModel();
	}
}

void RenderSceneStencil() {
	for (auto element = mainScene.objects.begin(); element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.000525f, 0.000525f, 0.000525f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		(*element).RenderModel();
	}
}

void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();
	
	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight *light) {
	omniShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightLocation();
	uniformFarPlane = omniShadowShader.GetFarPlanelocation();
	
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomBlurPass()
{

}

void RenderToQuadApplyBloom() 
{
	// could make framebuffer / dualframebuffer children of a base class and treat them in a generic manner for automatic use
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // switch this to the other dual fbo thing and then do a final render to quad

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	dualFramebuffershader.UseShader();
	dualFramebuffershader.SetGamma(gamma);


	dualFramebufferHDR.BindTexture(0);
	dualFramebufferHDR.BindTexture(1);
	dualFramebuffershader.SetTextureScreenSpace(18);
	dualFramebuffershader.SetTextureScreenSpaceTwo(19);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}

void MainRenderSetup(unsigned short int i, glm::mat4 projectionMatrix, glm::mat4 viewMatrix,
						 unsigned short int* pointLightCount, unsigned short int* spotLightCount,
						 DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights)
{
	shaderList[i].UseShader();

	uniformModel = shaderList[i].GetModelLocation();
	uniformProjection = shaderList[i].GetProjectionLocation();
	uniformView = shaderList[i].GetViewLocation();
	uniformEyePosition = shaderList[i].GetEyePositionLocation();
	uniformEyeDirection = shaderList[i].GetEyeDirectionLocation();
	uniformSpecularIntensity = shaderList[i].GetSpecularIntensityLocation();
	uniformSpecularPower = shaderList[i].GetSpecularPowerLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	glUniform3f(uniformEyeDirection, camera.getCameraDirection().x, camera.getCameraDirection().y, camera.getCameraDirection().z);

	shaderList[i].SetSplitScreenIsOn(splitScreenIsOn);
	shaderList[i].SetSplitScreenType(splitScreenType);
	shaderList[i].SetGamma(gamma);
	shaderList[i].SetBloomThreshold(bloomThreshold);

	(*mainLight).GetShadowMap()->Read(GL_TEXTURE2); // 2
	shaderList[i].SetTextureDiffuse(1);
	shaderList[i].SetTextureSpecular (4);
	shaderList[i].SetTextureNormal(5);
	shaderList[i].SetTextureHeight(7);
	shaderList[i].SetDirectionalShadowMap(2); // 2
	shaderList[i].SetTextureSkyBox(6);	
	shaderList[i].SetDirectionalLight(mainLight);
	shaderList[i].SetPointLights(pointLights, (*pointLightCount), 8, 0);
	shaderList[i].SetSpotLights(spotLights, (*spotLightCount), 8 + (*pointLightCount), (*pointLightCount));
	shaderList[i].SetDirectionalLightTransform(&(*mainLight).CalculateLightTransform());


	shaderList[i].Validate();
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, 
				unsigned short int* pointLightCount, unsigned short int* spotLightCount,
				DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights) {

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dualFramebufferHDR.GetFBO());
	
	glViewport(0, 0, 3840, 2160);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	MainRenderSetup(currentShader, projectionMatrix, viewMatrix, pointLightCount, spotLightCount, mainLight, pointLights, spotLights);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	mainScene.GetSkyBoxPtr()->bindCubeMapTexture(); // TEXTURE UNIT 6

	RenderScene();	

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	mainScene.GetSkyBoxPtr()->DrawSkyBox(viewMatrix, projectionMatrix, gamma, bloomThreshold);

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

	// BloomBlurPass();

	RenderToQuadApplyBloom(); // make it take the dualFrameBuffer object


	// add bloom with the blur n stuff
	/*
	
	
		
	
	
	*/
	

	/*
	// this will be the final render 


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	dualFramebuffershader.UseShader();
	dualFramebuffershader.SetGamma(gamma);
	dualFramebuffershader.SetTextureScreenSpace(18);
	dualFramebuffershader.SetTextureScreenSpaceTwo(19);
	dualFramebufferHDR.BindTextures(GL_TEXTURE17, GL_TEXTURE18);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);

	*/

}

void CreateLights(PointLight &pointLightsR, 
				  SpotLight &spotLightsR, 
				  unsigned short int *pointLightCount, 
				  unsigned short int *spotLightCount) {

	// make plain text loader for lights, add to scene.
	// added shadow detail var for ease testing
	GLint shadowDetail = 4;

	PointLight *pointLights = &pointLightsR; 
	SpotLight *spotLights = &spotLightsR;

	
	pointLights[0] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		0.7f, 0.7f, 1.0f,
		0.001f, 40.0f,
		3.0f, 2.75f, 3.0f,
		100.0f, 10.0f, -200.8f);
	(*pointLightCount)++;
	pointLights[0].SetLightRange(12.0f);

	pointLights[1] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		1.0f, 0.7f, 0.7f,
		0.0001f, 1.0f,
		8.0f, 8.0f, -0.5f,
		1.0f, 0.7f, -1.8f);
	(*pointLightCount)++;
	pointLights[1].SetLightRange(12.0f);
/*
	pointLights[2] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		0.7f, 1.0f, 0.7f,
		0.0001f, 1.0f,
		8.0f, 8.0f, 0.5f,
		-1.0f, 0.7f, 1.8f);
	(*pointLightCount)++;
	pointLights[2].SetLightRange(12.0f);

	spotLights[0] = SpotLight(shadowDetail * 2048, shadowDetail * 2048,
		0.1f, 100.0f,
		0.5f, 0.5f, 0.5f,						// FIX SPOTLIGHT EDGE < < < < < << << << <<< <<< <<<< <<<< <<<<< <<<<<<
		0.01f, 0.9f,
		5.0f, 4.0f, 1.0f,
		-0.9f, -1.0f, -0.40f,
		0.8f, 0.1f, 0.01f,
		32.0f); 
	(*spotLightCount)++;
	spotLights[0].SetLightRange(80.0f);

*/	
}

GLfloat DeltaTime() 
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	return deltaTime;
}




int main() 
{	
//<>=========================================================================================================<>
// Window Setup

	mainWindow = GL_Window(3840, 2160);
	mainWindow.Initialize();

	glViewport(0, 0, 3840, 2160);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

// additional settings
	glfwWindowHint(GLFW_SAMPLES, 16);
	glEnable(GL_MULTISAMPLE);
	glfwSwapInterval(1); // vsync
	mainWindow.swapBuffers();
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_FRAMEBUFFER_SRGB);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//<>=========================================================================================================<>
// frame buffer setup

	float quadVertices[] = 
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	framebufferHDR.Init(GL_TEXTURE17, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);

	dualFramebufferHDR.Init(GL_TEXTURE18, GL_TEXTURE19, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);

//<>=========================================================================================================<>
// prep

	DirectionalLight mainLight;
	glm::mat4 projection;

	camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90.0f, 0, 3.0f, 0.5f);
	projection = glm::perspective(glm::radians(52.0f), mainWindow.getAspectRatio(), 0.1f, 100.0f);

	shineMaterial = Material(4.0f, 32.0f);
	dullMaterial = Material(0.5f, 50.0f);

	mainLight = DirectionalLight(256, 256,
								 0.01f, 0.01f, 0.01f,
								 0.25f, 0.8f,
								 -5.0f, -10.0f, -10.0f);

	CreateLights(*pointLights, *spotLights, &pointLightCount, &spotLightCount);
	CreateShaders();
	mainScene.loadNewFileLocation("Models/default.FLS");
	mainScene.skybox = SkyBox("Textures/skybox", "Shaders/skybox.vert", "Shaders/skybox.frag");
   
//<>=========================================================================================================<>
// (immediate mode graphic user interface)  ---------  IMGUI

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	io.Fonts->Clear();

	ImFont* font = io.Fonts->AddFontFromFileTTF(u8"misc/fonts/Karla-Regular.ttf", 22.0f);
	
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(mainWindow.mainWindow, true);
	ImGui_ImplOpenGL3_Init(u8"#version 430");

	GraphicUI graphicUserInterface = GraphicUI(mainWindow.mainWindow);

//<>=========================================================================================================<>
// MAIN LOOP

	GLfloat spinModifier = 0.0f;

	while (!mainWindow.getShouldClose()) {
		
		// Other scene variables
		deltaTime = DeltaTime();

			// junk JUNK
			glm::vec3 ALP = camera.getCameraPosition();
			ALP = glm::vec3(ALP.x, ALP.y - 0.275f, ALP.z + 0.275f);
			pointLights[1].SetPosition(ALP);
			// junk JUNK

		spin += spinModifier;
		if (spin >= 360.00f) { spin = 0.0f; }

		// Main GL Calls
		glfwPollEvents();		
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.keyControl(mainWindow.getKeys(), deltaTime);

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++) { OmniShadowMapPass(&pointLights[i]); }
		for (size_t i = 0; i < spotLightCount; i++) { OmniShadowMapPass(&spotLights[i]); }

		RenderPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);

		glUseProgram(0);

		// GUI
		graphicUserInterface.Start();
		ImGui::ShowStyleEditor();

		graphicUserInterface.EditLights(pointLights, NULL, NULL, pointLightCount, NULL, false, true, false);
		graphicUserInterface.EditScene(&spinModifier);
		graphicUserInterface.EditRenderSettings(&splitScreenIsOn, &splitScreenType, &gamma, &bloomThreshold);
		graphicUserInterface.DisplayInfo();
		
		graphicUserInterface.End();

		// DisplayFinalFrame
		mainWindow.swapBuffers();


	}
//<>=========================================================================================================<>
// EXIT

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow.mainWindow);
	glfwTerminate();

	return 0;
}

/*





	vec2 offsets[9] = vec2[](
		 vec2(-offset , offset),
		 vec2( 0.0f   , offset),
		 vec2( offset , offset),
		 vec2(-offset , 0.0f  ),
		 vec2( 0.0f   , 0.0f  ),
		 vec2( offset , 0.0f  ),
		 vec2(-offset ,-offset),
		 vec2( 0.0f   ,-offset),
		 vec2( offset ,-offset)
	);

	float kernel[9] = float[](
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(theTextureDiffuse, TexCoords.st + offsets[i]));
	}

	vec3 col = vec3(0.0f);
	for (int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}

	vec4 brightnessf = vec4(texture(theTextureDiffuse, TexCoords).xyz, 1.0f);
	float brightness = (brightnessf.x + brightnessf.y + brightnessf.z) / 3.0f;
	//FragColor = ( (vec4(col, 1.0f) * texture(theTextureDiffuse, TexCoords) ) - (1.0f - brightness)) + texture(theTextureDiffuse,TexCoords);







*/