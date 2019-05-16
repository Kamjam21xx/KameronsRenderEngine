#pragma

//										 openGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//										STB image loader
#define STB_IMAGE_IMPLEMENTATION
// #define KAM_MATH_LIBRARY
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
//#include "KML.h"
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
#include "MultiFrameBuffer.h"
#include "GBuffer.h"

// linear interpolation == free lunch
// possibly make a bokeh bloom blur shader that takes advantage of linear interpolation to keep it light

// make a settings class maybe

// take some time to rename things later and do some proper inheritance

const float toRadians = 3.14159265 / 180.0;

GL_Window mainWindow;
FrameBuffer framebufferBlur;
MultiFrameBuffer dualFramebufferHDR;
MultiFrameBuffer blurBuffer;
GBuffer gBuffer;

Camera camera;
Scene mainScene;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader outlineShader;
Shader framebufferBlurShader;
Shader dualFramebuffershader;
Shader gBufferShader;
Shader quadShaderDeferred;

GLuint uniformProjection = 0,
	   uniformModel = 0,
	   uniformView = 0,
	   uniformEyePosition = 0,
	   uniformEyeDirection = 0,
	   uniformSpecularIntensity = 0,
	   uniformSpecularPower = 0,
	   uniformOmniLightPos = 0,
	   uniformFarPlane = 0,
	   uniformInverseProjection = 0,
	   uniformInverseView = 0;

unsigned short int currentShader = 0;

// main shader program hard locations
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";
static const char* olvShader = "Shaders/olfshader.vert";
static const char* olfShader = "Shaders/olfshader.frag";
static const char* outlineVShader = "Shaders/outline.vert";
static const char* outlineFShader = "Shaders/outline.frag";
static const char* FBBVShader = "Shaders/framebufferBlur.vert";
static const char* FBBFShader = "Shaders/framebufferBlur.frag";
static const char* DFBVShader = "Shaders/dualFramebuffershader.vert";
static const char* DFBFShader = "Shaders/dualFramebuffershader.frag";
static const char* gBufferShaderV = "Shaders/mainDeferredShader.vert";
static const char* gBufferShaderF = "Shaders/mainDeferredShader.frag";
static const char* quadShaderDeferredV = "Shaders/quadShaderDeferred.vert";
static const char* quadShaderDeferredF = "Shaders/quadShaderDeferred.frag";

Material shineMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;	
GLfloat now = 0.0f;
GLfloat spin;

bool forwardRender = true;
GLboolean splitScreenIsOn = false;
GLuint splitScreenType = 0;
GLfloat gamma = 1.0f;
GLfloat bloomThreshold = 1.0f;
unsigned short int numOfBloomPasses = 5;
GLfloat brightness = 0.0f;
GLfloat contrast = 1.0f;
GLfloat saturation = 0.5f;
GLfloat height = 1.0f;

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned short int spotLightCount = 0;
unsigned short int pointLightCount = 0;

unsigned int screenQuadVAO, screenQuadVBO;
int TEST_VALUE_IMGUI = 0;












void errorCheckGL(std::string location)
{
	GLenum error;
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string errorType = "";

		switch (error)
		{
		case 1280: errorType = "invalid enum";
			break;
		case 1281: errorType = "invalid parameter";
			break;
		case 1282: errorType = "illegal command";
			break;
		case 1283: errorType = "stack overflow";
			break;
		case 1284: errorType = "stack underflow";
			break;
		case 1285: errorType = "out of memory";
			break;
		case 1286: errorType = "read/write to incomplete FBO";
			break;
		}

		std::cout << location << " : ERROR : " << errorType << std::endl;
	}
}
glm::mat4 calculateInverseProjection(glm::mat4 projectionMatrix)
{
	glm::mat4 inverseProjection = glm::mat4(0.0f);

	inverseProjection[0][0] = 1.0f / projectionMatrix[0][0];
	inverseProjection[1][1] = 1.0f / projectionMatrix[1][1];
	inverseProjection[2][3] = 1.0f / projectionMatrix[3][2];
	inverseProjection[3][2] = 1.0f / projectionMatrix[2][2];
	inverseProjection[3][3] = projectionMatrix[2][2] / (projectionMatrix[3][2] * projectionMatrix[2][3]);

	return inverseProjection;
}
void CreateShaders() 
{
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

	framebufferBlurShader = Shader();
	framebufferBlurShader.CreateFromFiles(FBBVShader, FBBFShader);

	dualFramebuffershader = Shader();
	dualFramebuffershader.CreateFromFiles(DFBVShader, DFBFShader);

	gBufferShader = Shader();
	gBufferShader.CreateFromFiles(gBufferShaderV, gBufferShaderF);

	quadShaderDeferred = Shader();
	quadShaderDeferred.CreateFromFiles(quadShaderDeferredV, quadShaderDeferredF);
}
void RenderScene() 
{
	for (auto element = mainScene.objects.begin() ; element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);
		model = glm::rotate(model, spin * toRadians, glm::vec3(0.0, 1.0, 0.0));
		//model = glm::translate(model, glm::vec3(2.0, 0.85, 3.0));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shineMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularPower); // make material pointers part of the models

		(*element).RenderModel();
	}
}
void RenderSceneStencil() 
{
	for (auto element = mainScene.objects.begin(); element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.000525f, 0.000525f, 0.000525f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		(*element).RenderModel();
	}
}
void DirectionalShadowMapPass(DirectionalLight* light) 
{
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
void OmniShadowMapPass(PointLight *light) 
{
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
void BloomBlurPass() // re-write to blur then downsize then blur etc and pack everything in a neat class or something
{
	unsigned short int iterations = 2 * numOfBloomPasses - 1;
	bool isHorizontal = true;
	bool first_iteration = true;

	framebufferBlurShader.UseShader();

	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer.GetFBO(isHorizontal));
	framebufferBlurShader.SetHorizontal(isHorizontal);
	dualFramebufferHDR.BindTexture(1);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);

	isHorizontal = !isHorizontal;

	glViewport(0, 0, 1920, 1080);
	for (unsigned short int i = 0; i < iterations; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer.GetFBO(isHorizontal));
		framebufferBlurShader.SetHorizontal(isHorizontal);

		blurBuffer.BindTexture(!isHorizontal);
		
		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);

		isHorizontal = !isHorizontal;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 3840, 2160);
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
	dualFramebuffershader.SetTextureScreenSpaceTwo(17);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
void RenderToQuadDeferred(glm::mat4 inverseProjectionMatrix, glm::mat4 inverseViewMatrix)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 

	glClearColor(0.1f, 0.1f, 0.9f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	quadShaderDeferred.UseShader();

	quadShaderDeferred.SetInverseProjection(&inverseProjectionMatrix);
	quadShaderDeferred.SetInverseView(&inverseViewMatrix);


	quadShaderDeferred.SetTextureDepth(23);
	quadShaderDeferred.SetTextureScreenSpace(TEST_VALUE_IMGUI);
	quadShaderDeferred.SetTextureScreenSpaceTwo(21);
	quadShaderDeferred.SetTextureScreenSpaceThree(20);

	quadShaderDeferred.SetPointLights(pointLights, pointLightCount, 8, 0);

	quadShaderDeferred.SetGamma(gamma);
	quadShaderDeferred.SetBrightness(brightness);
	quadShaderDeferred.SetContrast(contrast);
	quadShaderDeferred.SetSaturation(saturation);


	gBuffer.BindTexturePos(20);
	gBuffer.BindTextureNormHeight(21);
	gBuffer.BindTextureColSpec(22);
	gBuffer.BindTextureDepthStencil(23);

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
	shaderList[i].SetBrightness(brightness);
	shaderList[i].SetContrast(contrast);
	shaderList[i].SetSaturation(saturation);
	shaderList[i].SetHeightPOM(height);


	(*mainLight).GetShadowMap()->Read(GL_TEXTURE2); // 2
	shaderList[i].SetTextureDiffuse(1);
	shaderList[i].SetTextureNormal(5);
	shaderList[i].SetDirectionalShadowMap(2); // 2
	shaderList[i].SetTextureSkyBox(6);	
	shaderList[i].SetDirectionalLight(mainLight);
	shaderList[i].SetPointLights(pointLights, (*pointLightCount), 8, 0);
	shaderList[i].SetSpotLights(spotLights, (*spotLightCount), 8 + (*pointLightCount), (*pointLightCount));
	shaderList[i].SetDirectionalLightTransform(&(*mainLight).CalculateLightTransform());


	shaderList[i].Validate();
}
void ForwardRenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, 
				unsigned short int* pointLightCount, unsigned short int* spotLightCount,
				DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights) 
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dualFramebufferHDR.GetFBO(0));
	
	glViewport(0, 0, 3840, 2160); // maybe make it a lower upscaled resolution. idk.
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

	// add shader swapping in ImGui 
	BloomBlurPass();

	RenderToQuadApplyBloom();
}
void GBufferSetup(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	gBufferShader.UseShader();
	gBuffer.BindAll();


	uniformModel = gBufferShader.GetModelLocation();
	uniformProjection = gBufferShader.GetProjectionLocation();
	uniformView = gBufferShader.GetViewLocation();
	uniformEyePosition = gBufferShader.GetEyePositionLocation();
	uniformEyeDirection = gBufferShader.GetEyeDirectionLocation();
	
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	


	gBufferShader.SetHeightPOM(height);

	gBufferShader.SetTextureDiffuse(unsigned short int(1));
	gBufferShader.SetTextureNormal(unsigned short int(5));
	gBufferShader.SetTextureSkyBox(unsigned short int(6));




	gBufferShader.Validate();


	
}
void DeferredRenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, unsigned short int *pointLightCount, unsigned short int *spotLightCount, DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer.GetFBO());
	glViewport(0, 0, 3840, 2160);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	// draw and set stencil mask

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);


	GBufferSetup(projectionMatrix, viewMatrix);	// make a member possibly


	glDisable(GL_STENCIL_TEST);

	RenderScene();

	glDisable(GL_DEPTH_TEST);

/*
	// flip stencil mask behavior
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	// draw skybox
	mainScene.GetSkyBoxPtr()->bindCubeMapTexture();					// TEXTURE UNIT 6

	// disable stencil and depth. passing position instead of calculating it from depth
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);
*/


	// calculate lighting, etc. get highlight and color
	//RenderLighting();												// pass light stuff

	// blur the bloom highlight // pass texture


	// combine bloom with color and do post processing
	glClear(GL_STENCIL_BUFFER_BIT);

	RenderToQuadDeferred(glm::inverse(projectionMatrix), glm::inverse(viewMatrix));

	glEnable(GL_DEPTH_TEST);




	// set everything back to stop imgui from breaking



	




}
void CreateLights(PointLight &pointLightsR, 
				  SpotLight &spotLightsR, 
				  unsigned short int *pointLightCount, 
				  unsigned short int *spotLightCount) 
{
	// make plain text loader for lights, add to scene.
	// added shadow detail var for ease testing
	GLint shadowDetail = 4;

	PointLight *pointLights = &pointLightsR; 
	SpotLight *spotLights = &spotLightsR;

	
	pointLights[0] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		0.7f, 0.7f, 1.0f,
		0.001f, 40.0f,
		3.0f, 8.0f, 3.0f,
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

	pointLights[2] = PointLight(shadowDetail * 256, shadowDetail * 256,
		0.01f, 100.0f,
		0.7f, 1.0f, 0.7f,
		0.0001f, 1.0f,
		8.0f, 8.0f, 0.5f,
		-1.0f, 0.7f, 1.8f);
	(*pointLightCount)++;
	pointLights[2].SetLightRange(12.0f);
/*
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


	framebufferBlur.Init(19, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);
	dualFramebufferHDR.Init(18, 19, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);
	blurBuffer.InitPingPong(17, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 1920, 1080);

	gBuffer.Init(20, 21, 22, 23, 3840, 2160);

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

	while (!mainWindow.getShouldClose()) 
	{
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



		if (forwardRender)
		{
			// FORWARD RENDERING
			DirectionalShadowMapPass(&mainLight);

			for (size_t i = 0; i < pointLightCount; i++) { OmniShadowMapPass(&pointLights[i]); }
			for (size_t i = 0; i < spotLightCount; i++) { OmniShadowMapPass(&spotLights[i]); }

			ForwardRenderPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);
		}
		else
		{
			// deffered rendering
			DeferredRenderPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);
		}

		

		

		glUseProgram(0);

		// GUI
		graphicUserInterface.Start();
		ImGui::ShowStyleEditor();

		ImGui::Begin("TEST");
		ImGui::SliderInt("TEST VALUE INT", &TEST_VALUE_IMGUI, 0, 30);
		ImGui::End();

		graphicUserInterface.EditLights(pointLights, NULL, NULL, pointLightCount, NULL, false, true, false);
		graphicUserInterface.EditScene(&spinModifier);
		graphicUserInterface.EditRenderSettings(&forwardRender, &height, &splitScreenIsOn, &splitScreenType, &gamma, &bloomThreshold, &brightness, &contrast, &saturation);
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



/*







// light
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform eyePosition;

// post
uniform float gammaLevel;
uniform float bloomThreshold;
uniform float brightness;
uniform float contrast;
uniform float saturation;

// structs
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;

	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float specularPower;
};












*/