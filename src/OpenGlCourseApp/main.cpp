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




/*
	making changes thanks to code reviews. if they arent here yet, they will be. 

	will add license info, plz dont sue me or steal my project.0
*/

const float toRadians = 3.14159265 / 180.0;

GL_Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Shader directionalShadowShader;
Shader omniShadowShader;
Shader outlineShader;
Shader framebuffershader;

Camera camera;

Material shineMaterial;
Material dullMaterial;

SkyBox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat spin;

GLuint uniformProjection = 0,
	   uniformModel = 0,
	   uniformView = 0,
	   uniformEyePosition = 0,
	   uniformEyeDirection = 0,
	   uniformSpecularIntensity = 0,
	   uniformSpecularPower = 0,
	   uniformOmniLightPos = 0,
	   uniformFarPlane = 0;

Scene mainScene;

GLboolean splitScreenIsOn = false;
GLuint splitScreenType = 0;


// main shader programs
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";
static const char* olvShader = "Shaders/olfshader.vert";
static const char* olfShader = "Shaders/olfshader.frag";
static const char* outlineVShader = "Shaders/outline.vert";
static const char* outlineFShader = "Shaders/outline.frag";
static const char* FBVShader = "Shaders/framebuffershader.vert";
static const char* FBFShader = "Shaders/framebuffershader.frag";

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned short int spotLightCount = 0;
unsigned short int pointLightCount = 0;




void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices,
						unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, 
				glm::mat4 viewMatrix, 
				unsigned short int* pointLightCount, 
				unsigned short int* spotLightCount, 
				DirectionalLight* mainLight, 
				PointLight *pointLights, 
				SpotLight *spotLights) {

	glViewport(0, 0, 3840, 2160);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	
	glStencilMask(0x00);


	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformEyeDirection = shaderList[0].GetEyeDirectionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformSpecularPower = shaderList[0].GetSpecularPowerLocation();

	//spotLights[0].SetFlash(camera.getCameraPosition(), camera.getCameraDirection());
	//pointLights[0].SetPosition(camera.getCameraPosition());
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	glUniform3f(uniformEyeDirection, camera.getCameraDirection().x, camera.getCameraDirection().y, camera.getCameraDirection().z);
	shaderList[0].SetSplitScreenIsOn(splitScreenIsOn);
	shaderList[0].SetSplitScreenType(splitScreenType);

	(*mainLight).GetShadowMap()->Read(GL_TEXTURE2); // 2
	shaderList[0].SetTextureDiffuse(1);
	shaderList[0].SetTextureSpecular (4);
	// shaderList[0].SetTextureMetal(8); 
	shaderList[0].SetTextureNormal(5);
	shaderList[0].SetTextureHeight(7);
	shaderList[0].SetDirectionalShadowMap(2); // 2
	shaderList[0].SetTextureSkyBox(6);	
	
	shaderList[0].SetDirectionalLight(mainLight);
	shaderList[0].SetPointLights(pointLights, (*pointLightCount), 8, 0);
	shaderList[0].SetSpotLights(spotLights, (*spotLightCount), 8 + (*pointLightCount), (*pointLightCount));
	shaderList[0].SetDirectionalLightTransform(&(*mainLight).CalculateLightTransform());

	shaderList[0].Validate();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	skybox.bindCubeMapTexture(); // TEXTURE UNIT 6
	RenderScene();	
	skybox.unbinedCubeMapTexture();


	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	skybox.DrawSkyBox(viewMatrix, projectionMatrix);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

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
/*
	pointLights[1] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		1.0f, 0.7f, 0.7f,
		0.0001f, 1.0f,
		8.0f, 8.0f, -0.5f,
		1.0f, 0.7f, -1.8f);
	(*pointLightCount)++;
	pointLights[1].SetLightRange(12.0f);

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






int main() 
{	
	mainWindow = GL_Window(3840, 2160);
	mainWindow.Initialize();

	glViewport(0, 0, 3840, 2160);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	

	glfwWindowHint(GLFW_SAMPLES, 16);
	glEnable(GL_MULTISAMPLE);
	glfwSwapInterval(1); // vsync
	mainWindow.swapBuffers();
	glEnable(GL_DEBUG_OUTPUT);

//<>=========================================================================================================<>

	DirectionalLight mainLight;
	std::vector<std::string> skyboxFaces;
	glm::mat4 projection;


	camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90.0f, 0, 3.0f, 0.5f);
	projection = glm::perspective(glm::radians(52.0f), mainWindow.getAspectRatio(), 0.1f, 100.0f);


	shineMaterial = Material(4.0f, 32.0f);
	dullMaterial = Material(0.5f, 50.0f);

	skyboxFaces.push_back("Textures/skybox/right.jpg"); // take note of the file type
		
	skyboxFaces.push_back("Textures/skybox/left.jpg");
		
	skyboxFaces.push_back("Textures/skybox/top.jpg");
		
	skyboxFaces.push_back("Textures/skybox/bot.jpg");
		
	skyboxFaces.push_back("Textures/skybox/back.jpg");
		
	skyboxFaces.push_back("Textures/skybox/front.jpg");


	GLfloat now;

	mainLight = DirectionalLight(256, 256,
								 0.01f, 0.01f, 0.01f,
								 0.25f, 0.8f,
								 -5.0f, -10.0f, -10.0f);

	//PointLight pointLights[MAX_POINT_LIGHTS];
	//SpotLight spotLights[MAX_SPOT_LIGHTS];
	//unsigned int spotLightCount = 0;
	//unsigned int pointLightCount = 0;
	CreateLights(*pointLights, *spotLights, &pointLightCount, &spotLightCount);
	CreateShaders();


	skybox = SkyBox(skyboxFaces); 
    mainScene.load();


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_FRAMEBUFFER_SRGB);

//<>=========================================================================================================<>
// (immediate mode graphic user interface)  ---------  IMGUI
//<>=========================================================================================================<>
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
	
	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

	GLfloat spinModifier = 0.0f;

//<>=========================================================================================================<>
	// MAIN LOOP
//<>=========================================================================================================<>
	while (!mainWindow.getShouldClose()) {
		
		// Other scene variables
		now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		spin += spinModifier;
		if (spin >= 360.00f) { spin = 0.0f; }

		// Main GL Calls
		glViewport(0, 0, 3840, 2160);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwPollEvents();		
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange()); // fix later to stop xchange while not clicked
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