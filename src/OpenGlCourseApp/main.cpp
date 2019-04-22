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

/*	 gotta give thanks to the Graphics Programming discord group for helping me learn and fix things when im stuck

	to do list
	- CODE REVIEW! when most of this is done

	x eliminate direct paths
	x encapsulate loading for skybox and simplify functions/construction
	x encapsulate skybox inside scene class
	x check and eliminate win lib include

	- add GetSkyBox() to the scene class
	- change include paths - per deccers advice
	- fix error checks printf %s with 2 parameters is wrong - per deccers advice
	- stop xChange and yChange for mouse input while not clicked, if first clicked, change is 0.0f
	- make materials encapsulate references to textures 
	- make texture packing functionality for faster gpu stuff 
	- eliminate matrix multiplication in vertex shader for faster execution speed - per jodies advice
	- make the scene class encapsulate more to clean up the code and make it more understandable
	- finish shader baker class "ShaderHandler" and work it in
	- setup FrameBuffer class
	- setup more GUI functionality 
	- make GUI more efficient and pretty with tabs
	- clean shaders
	- pack texture data into fewer texture units with no unused channels
	- add metal to scene, model, texture, shaders and shader class
	- fix directional light and fix to camera
	- fix spotlight edge calculations
	- setup hdri and post processing buffer
	- setup deffered rendering
	- add pbr mode, pipeline, shaders, and GUI manip
	- implement fast fourier transform
	- implement reflection
	- reinstate alpha and blending and add sort algorithm and seperate shader 
	- implement auto alpha detection for if the alpha channel is used, and use correct texture loader. flip bit flag for hasAlpha
	- add mouse smoothing WHILE CONTROLING CAMERA
	- add particles
	- add voxel based volume rendering
	- add voxel based fluid simulation
	- add particle simulation
	- create water shader with FFT
	- add hotkey/keyBinding editor
	- add smooth acceleration to camera movement
	- add sort algorithm for baked shader functionality
	- add audio engine
	- add event handling
	- add physics engine
	
	CLEAN UP MAIN
	+ much more

*/

/*
	making changes thanks to code reviews. if they arent here yet, they will be. 

	will add license info, plz dont sue me or steal my project.0
*/

const float toRadians = 3.14159265 / 180.0;

GL_Window mainWindow;
FrameBuffer framebufferHDR;
Camera camera;

Scene mainScene;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader outlineShader;
Shader framebuffershader;

GLuint uniformProjection = 0,
	   uniformModel = 0,
	   uniformView = 0,
	   uniformEyePosition = 0,
	   uniformEyeDirection = 0,
	   uniformSpecularIntensity = 0,
	   uniformSpecularPower = 0,
	   uniformOmniLightPos = 0,
	   uniformFarPlane = 0;

// main shader program hard locations
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";
static const char* olvShader = "Shaders/olfshader.vert";
static const char* olfShader = "Shaders/olfshader.frag";
static const char* outlineVShader = "Shaders/outline.vert";
static const char* outlineFShader = "Shaders/outline.frag";
static const char* FBVShader = "Shaders/framebuffershader.vert";
static const char* FBFShader = "Shaders/framebuffershader.frag";

Material shineMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;	
GLfloat now = 0.0f;
GLfloat spin;

GLboolean splitScreenIsOn = false;
GLuint splitScreenType = 0;

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned short int spotLightCount = 0;
unsigned short int pointLightCount = 0;

unsigned int screenQuadVAO, screenQuadVBO;


void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 normal;
	unsigned int in0;
	unsigned int in1;
	unsigned int in2;

	for (size_t i = 0; i < indiceCount; i += 3) {
		in0 = indices[i + 0] * vLength;
		in1 = indices[i + 1] * vLength;
		in2 = indices[i + 2] * vLength;
		
		v1 = glm::vec3(vertices[in1 + 0] - vertices[in0 + 0],
					   vertices[in1 + 1] - vertices[in0 + 1],
					   vertices[in1 + 2] - vertices[in0 + 2]
		);
		v2 = glm::vec3(vertices[in2 + 0] - vertices[in0 + 0],  
					   vertices[in2 + 1] - vertices[in0 + 1],
					   vertices[in2 + 2] - vertices[in0 + 2]
		);
		
		normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x ; vertices[in0 + 1] += normal.y ; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x ; vertices[in1 + 1] += normal.y ; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x ; vertices[in2 + 1] += normal.y ; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset; 
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset + 0] = vec.x; 
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
} // phong average

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
	//glBindFramebuffer(GL_FRAMEBUFFER, framebufferHDR.FBO);
}

void OmniShadowMapPass(PointLight *light) {
	omniShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();


	glClear(GL_DEPTH_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightLocation();
	uniformFarPlane = omniShadowShader.GetFarPlanelocation();
	
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();
	//glBindFramebuffer(GL_FRAMEBUFFER, framebufferHDR.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, 
				glm::mat4 viewMatrix, 
				unsigned short int* pointLightCount, 
				unsigned short int* spotLightCount, 
				DirectionalLight* mainLight, 
				PointLight *pointLights, 
				SpotLight *spotLights) {

																			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferHDR.FBO);
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 3840, 2160);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformEyeDirection = shaderList[0].GetEyeDirectionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformSpecularPower = shaderList[0].GetSpecularPowerLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	glUniform3f(uniformEyeDirection, camera.getCameraDirection().x, camera.getCameraDirection().y, camera.getCameraDirection().z);

	shaderList[0].SetSplitScreenIsOn(splitScreenIsOn);
	shaderList[0].SetSplitScreenType(splitScreenType);
																			// (*mainLight).GetShadowMap()->Read(GL_TEXTURE2); // 2
	shaderList[0].SetTextureDiffuse(1);
	shaderList[0].SetTextureSpecular (4);
	// shaderList[0].SetTextureMetal(8); 
	shaderList[0].SetTextureNormal(5);
	shaderList[0].SetTextureHeight(7);
	shaderList[0].SetDirectionalShadowMap(2); // 2
	shaderList[0].SetTextureSkyBox(6);	
																			// shaderList[0].SetDirectionalLight(mainLight);
																			// shaderList[0].SetPointLights(pointLights, (*pointLightCount), 8, 0);
																			// shaderList[0].SetSpotLights(spotLights, (*spotLightCount), 8 + (*pointLightCount), (*pointLightCount));
																			// shaderList[0].SetDirectionalLightTransform(&(*mainLight).CalculateLightTransform());
	shaderList[0].Validate();
	// shaderList[0].SetTextureScreenSpace(18);

/**/

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

																			// mainScene.skybox.bindCubeMapTexture(); // TEXTURE UNIT 6

	RenderScene();	

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	// framebufferHDR.BindTexture(GL_TEXTURE16);
																			// mainScene.skybox.DrawSkyBox(viewMatrix, projectionMatrix);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

/**/

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

																			framebuffershader.UseShader();
																			
																			framebuffershader.SetTextureScreenSpace(18);
																			framebufferHDR.BindTexture(GL_TEXTURE18);
	glBindVertexArray(screenQuadVAO);
	glBindTexture(GL_TEXTURE_2D, framebufferHDR.texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);

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
	//glfwWindowHint(GLFW_SAMPLES, 16);
	//glEnable(GL_MULTISAMPLE);
	//glfwSwapInterval(1); // vsync
	mainWindow.swapBuffers();
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_FRAMEBUFFER_SRGB);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//<>=========================================================================================================<>
// frame buffer setup

	float quadVertices[] = {
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

	// framebufferHDR = FrameBuffer(GL_TEXTURE15, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_LINEAR, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
	framebufferHDR = FrameBuffer(GL_TEXTURE16, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, 3840, 2160);

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

	ImFont* font = io.Fonts->AddFontFromFileTTF(u8"C:\\Users\\Kameron\\Documents\\GitHub\\KameronsRenderEngine\\src\\OpenGlCourseApp\\misc\\fonts\\Karla-Regular.ttf", 22.0f);
	
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
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferHDR.FBO);
		glViewport(0, 0, 3840, 2160);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
		graphicUserInterface.EditRenderSettings(&splitScreenIsOn, &splitScreenType);
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