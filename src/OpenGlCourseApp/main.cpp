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
#include <random>
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
#include "ScreenQuad.h"

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
Shader shaderDirectionalShadow;
Shader shaderOmniShadow;
Shader shaderOutline;
Shader shaderBlur;
Shader shaderForwardPost;
Shader shaderGBuffer;
Shader shaderDeferredPost;
Shader ShaderSSAO;

// main shader program hard locations
static const char* sourceForwardVert = "Shaders/shader.vert";
static const char* sourceForwardFrag = "Shaders/shader.frag";
static const char* sourceOutlinevert = "Shaders/outline.vert";
static const char* sourceOutlineFrag = "Shaders/outline.frag";
static const char* sourceBlurVert = "Shaders/framebufferBlur.vert";
static const char* sourceBlurFrag = "Shaders/framebufferBlur.frag";
static const char* sourceForwardPostVert = "Shaders/dualFramebuffershader.vert";
static const char* sourceForwardPostFrag = "Shaders/dualFramebuffershader.frag";
static const char* sourceGBufferVert = "Shaders/mainDeferredShader.vert";
static const char* sourceGBufferFrag = "Shaders/mainDeferredShader.frag";
static const char* sourceDeferredPostVert = "Shaders/quadShaderDeferred.vert";
static const char* sourceDeferredPostFrag = "Shaders/quadShaderDeferred.frag";
static const char* sourceSSAOVert= "Shaders/ssaoDeferred.vert";
static const char* sourceSSAOFrag = "Shaders/ssaoDeferred.frag";

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

Material shineMaterial;
Material dullMaterial;

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
GLfloat radiusAO = 0.25f;
GLfloat biasAO = 0.025f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;	
GLfloat now = 0.0f;
GLfloat spin;

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned short int spotLightCount = 0;
unsigned short int pointLightCount = 0;

ScreenQuad screenQuad;

int TEST_VALUE_IMGUI = 0;


// TEMP SSAO
std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
std::default_random_engine generator;

std::vector<glm::vec3> kernelSSAO; // keep kernel first for auto completion stuff
std::vector<glm::vec3> noiseSSAO;

Texture noiseTexture;
FrameBuffer ssaoBuffer;
// TEMP SSAO





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
GLfloat DeltaTime() 
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	return deltaTime;
}
void CreateShaders() 
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(sourceForwardVert, sourceForwardFrag);
	shaderList.push_back(*shader1);

	shaderOutline = Shader();
	shaderOutline.CreateFromFiles(sourceOutlinevert, sourceOutlineFrag);

	shaderDirectionalShadow = Shader();
	shaderDirectionalShadow.CreateFromFiles("Shaders/directional_shadow_map.vert", 
											"Shaders/directional_shadow_map.frag"
	);
	shaderOmniShadow = Shader();
	shaderOmniShadow.CreateFromFiles("Shaders/omni_shadow_map.vert",
											"Shaders/omni_shadow_map.geom",
											"Shaders/omni_shadow_map.frag"
	);

	shaderBlur = Shader();
	shaderBlur.CreateFromFiles(sourceBlurVert, sourceBlurFrag);

	shaderForwardPost = Shader();
	shaderForwardPost.CreateFromFiles(sourceForwardPostVert, sourceForwardPostFrag);

	shaderGBuffer = Shader();
	shaderGBuffer.CreateFromFiles(sourceGBufferVert, sourceGBufferFrag);

	shaderDeferredPost = Shader();
	shaderDeferredPost.CreateFromFiles(sourceDeferredPostVert, sourceDeferredPostFrag);

	ShaderSSAO = Shader();
	ShaderSSAO.CreateFromFiles(sourceSSAOVert, sourceSSAOFrag);
}
void CreateLights(PointLight &pointLightsR, SpotLight &spotLightsR, unsigned short int *pointLightCount, unsigned short int *spotLightCount) 
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
void RenderSceneStencil()  // Edit to use for individual objects as a subset of renderScene()
{
	for (auto element = mainScene.objects.begin(); element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.000525f, 0.000525f, 0.000525f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		(*element).RenderModel();
	}
}

void ForwardBloomBlurPass() // re-write to blur then downsize then blur etc and pack everything in a neat class or something
{
	unsigned short int iterations = 2 * numOfBloomPasses - 1;
	bool isHorizontal = true;
	bool first_iteration = true;

	shaderBlur.UseShader();

	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer.GetFBO(isHorizontal));
	shaderBlur.SetHorizontal((GLboolean*)&isHorizontal);
	dualFramebufferHDR.BindTexture(1);

	screenQuad.Render();

	glEnable(GL_DEPTH_TEST);

	isHorizontal = !isHorizontal;

	glViewport(0, 0, 1920, 1080);
	for (unsigned short int i = 0; i < iterations; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer.GetFBO(isHorizontal));
		shaderBlur.SetHorizontal((GLboolean*)&isHorizontal);

		blurBuffer.BindTexture(!isHorizontal);
		
		screenQuad.Render();
		glEnable(GL_DEPTH_TEST);

		isHorizontal = !isHorizontal;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 3840, 2160);
}
void ForwardFinalPass() 
{
	// could make framebuffer / dualframebuffer children of a base class and treat them in a generic manner for automatic use
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // switch this to the other dual fbo thing and then do a final render to quad

	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	shaderForwardPost.UseShader();
	shaderForwardPost.SetGamma(&gamma);

	dualFramebufferHDR.BindTexture(0);
	dualFramebufferHDR.BindTexture(1);
	shaderForwardPost.SetTextureScreenSpace(18);
	shaderForwardPost.SetTextureScreenSpaceTwo(17);

	screenQuad.Render();

	glEnable(GL_DEPTH_TEST);
}
void DeferredFinalPass(glm::mat4 *inverseProjectionMatrix, glm::mat4 *inverseViewMatrix)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 

	glClearColor(0.1f, 0.1f, 0.9f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	shaderDeferredPost.UseShader();

	shaderDeferredPost.SetInverseProjection(inverseProjectionMatrix);
	shaderDeferredPost.SetInverseView(inverseViewMatrix);

	shaderDeferredPost.SetTextureDepth(23);
	shaderDeferredPost.SetTextureScreenSpace(TEST_VALUE_IMGUI);
	shaderDeferredPost.SetTextureScreenSpaceTwo(21);
	shaderDeferredPost.SetTextureScreenSpaceThree(20);
	shaderDeferredPost.SetTextureAO(16);

	shaderDeferredPost.SetPointLights(pointLights, pointLightCount, 8, 0);

	shaderDeferredPost.SetGamma(&gamma);
	shaderDeferredPost.SetBrightness(&brightness);
	shaderDeferredPost.SetContrast(&contrast);
	shaderDeferredPost.SetSaturation(&saturation);
    
	ssaoBuffer.BindTexture(16);
	gBuffer.BindTextureNormHeight(21);
	gBuffer.BindTextureColSpec(22);
	gBuffer.BindTextureDepthStencil(23);
	
	screenQuad.Render();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
}

// main passes call other render passes
void ForwardMainPassSetup(unsigned short int i, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, unsigned short int* pointLightCount, unsigned short int* spotLightCount, DirectionalLight *mainLight, PointLight *pointLights, SpotLight *spotLights)
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


	shaderList[i].SetSplitScreenIsOn(&splitScreenIsOn);
	shaderList[i].SetSplitScreenType(&splitScreenType);
	shaderList[i].SetGamma(&gamma);
	shaderList[i].SetBloomThreshold(&bloomThreshold);
	shaderList[i].SetBrightness(&brightness);
	shaderList[i].SetContrast(&contrast);
	shaderList[i].SetSaturation(&saturation);
	shaderList[i].SetHeightPOM(&height);


	mainLight->GetShadowMap()->Read(GL_TEXTURE2); // 2
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
void ForwardMainPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, unsigned short int* pointLightCount, unsigned short int* spotLightCount, DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights) 
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dualFramebufferHDR.GetFBO(0));
	
	glViewport(0, 0, 3840, 2160); // maybe make it a lower upscaled resolution. idk.
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	ForwardMainPassSetup(currentShader, projectionMatrix, viewMatrix, pointLightCount, spotLightCount, mainLight, pointLights, spotLights);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	mainScene.GetSkyBoxPtr()->bindCubeMapTexture(); // TEXTURE UNIT 6

	RenderScene();	

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	mainScene.GetSkyBoxPtr()->DrawSkyBox(&viewMatrix, &projectionMatrix, &gamma, &bloomThreshold, &brightness, &contrast, &saturation);

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

	// add shader swapping in ImGui 
	ForwardBloomBlurPass();

	ForwardFinalPass();
}
void DeferredMainPassSetup(glm::mat4 *projectionMatrix, glm::mat4 *viewMatrix)
{
	shaderGBuffer.UseShader();
	gBuffer.BindAll();


	uniformModel = shaderGBuffer.GetModelLocation();
	uniformProjection = shaderGBuffer.GetProjectionLocation();
	uniformView = shaderGBuffer.GetViewLocation();
	uniformEyePosition = shaderGBuffer.GetEyePositionLocation();
	uniformEyeDirection = shaderGBuffer.GetEyeDirectionLocation();
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(*viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	shaderGBuffer.SetHeightPOM(&height);

	shaderGBuffer.SetTextureDiffuse(unsigned short int(1));
	shaderGBuffer.SetTextureNormal(unsigned short int(5));
	shaderGBuffer.SetTextureSkyBox(unsigned short int(6));

	shaderGBuffer.Validate();
}
void DeferredMainPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, unsigned short int *pointLightCount, unsigned short int *spotLightCount, DirectionalLight* mainLight, PointLight *pointLights, SpotLight *spotLights)
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


	DeferredMainPassSetup(&projectionMatrix, &viewMatrix);	// make a member possibly


	glDisable(GL_STENCIL_TEST);

	RenderScene();

	glDisable(GL_DEPTH_TEST);



	// flip stencil mask behavior
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);



	// disable stencil and depth. passing position instead of calculating it from depth
	glStencilMask(0xFF);
	glDisable(GL_STENCIL_TEST);

	// (GL_STENCIL_BUFFER_BIT);

	// SSAO
	// make a function to bake "samples[kernelSize]" to avoid uniform arrays
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBuffer.GetFBO());
	glViewport(0, 0, 3840, 2160);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderSSAO.UseShader();

	ShaderSSAO.SetTextureNoiseSSAO(24);
	ShaderSSAO.SetTextureDepth(23);
	ShaderSSAO.SetTextureScreenSpaceTwo(21);
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
	glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
	ShaderSSAO.SetInverseProjection(&inverseProjectionMatrix);	
	ShaderSSAO.SetRandomSamplesSSAO(&kernelSSAO);

	ShaderSSAO.SetAmbientOcclusionRadius(0.1 * radiusAO); // rework shader class and make it optional to pass a pointer too.
	ShaderSSAO.SetAmbientOcclusionBias(0.1 * biasAO);

	ShaderSSAO.Validate();

	gBuffer.BindTextureDepthStencil(23);
	gBuffer.BindAndSetTexturePos(20); // view space normal not position
	noiseTexture.BindTexture(24);

	screenQuad.Render();
	// SSAO


	// blur the bloom highlight // pass texture


	// combine bloom with color and do post processing

	//glClear(GL_STENCIL_BUFFER_BIT);
	DeferredFinalPass(&inverseProjectionMatrix, &inverseViewMatrix);

	
	// draw skybox
	glEnable(GL_STENCIL_TEST);
	//mainScene.GetSkyBoxPtr()->bindCubeMapTexture();					// TEXTURE UNIT 6
	//mainScene.GetSkyBoxPtr()->DrawSkyBoxDeferred(&viewMatrix, &projectionMatrix, &gamma, &bloomThreshold, &brightness, &contrast, &saturation);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

	// add highlight output
	// render sky here 
	// dont clear stencil
	// change stencil 
	// re-enable stencil


	glEnable(GL_DEPTH_TEST);
	// set everything back to stop imgui from breaking



	




}

void ShadowMapPassDirectional(DirectionalLight* light) 
{
	shaderDirectionalShadow.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = shaderDirectionalShadow.GetModelLocation();
	shaderDirectionalShadow.SetDirectionalLightTransform(&light->CalculateLightTransform());

	shaderDirectionalShadow.Validate();
	
	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMapPassOmni(PointLight *light) 
{
	shaderOmniShadow.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = shaderOmniShadow.GetModelLocation();
	uniformOmniLightPos = shaderOmniShadow.GetOmniLightLocation();
	uniformFarPlane = shaderOmniShadow.GetFarPlanelocation();
	
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	shaderOmniShadow.SetLightMatrices(light->GetPositionPtr(), light->GetLightProjectionPtr());

	shaderOmniShadow.Validate();

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	screenQuad.Init();

	framebufferBlur.Init(19, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);
	dualFramebufferHDR.Init(18, 19, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 3840, 2160);
	blurBuffer.InitPingPong(17, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 1920, 1080);

	gBuffer.Init(20, 21, 22, 23, 3840, 2160);

//<>=========================================================================================================<>
// prep

	DirectionalLight mainLight;
	glm::mat4 projection;

	camera = Camera(glm::vec3(0, 5, 11), glm::vec3(0, 1, 0), -90.0f, 0, 3.0f, 0.5f);
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
// SSAO prep
	// create random linear values between [0, 1]
	// normalize them
	// get a 2 quadrants so you got a hemisphere then
	// multiply it by a random number between [0, 1]
	for(unsigned short int i = 0; i < 64; ++i)
	{
		glm::vec3 sample( randomFloats(generator) * 2.0f - 1.0f,
						  randomFloats(generator) * 2.0f - 1.0f,
						  randomFloats(generator));

		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.0;
		scale = (0.1f + scale * scale) * 0.9f;
		sample *= scale;
		kernelSSAO.push_back(sample);
	}

	for (unsigned short int i = 0; i < 16; ++i)
	{
		glm::vec3 noise( randomFloats(generator) * 2.0f - 1.0f,
						 randomFloats(generator) * 2.0f - 1.0f,
						 0.0f);
	
		noiseSSAO.push_back(noise);
	}

	noiseTexture.LoadTextureData(&noiseSSAO[0], 4, 4, GL_RGB16F, GL_RGB, GL_FLOAT);
	noiseTexture.SetTextureUnit(GL_TEXTURE24);
	ssaoBuffer.Init(16, GL_RED, GL_RGB, GL_FLOAT, GL_NEAREST, 3840, 2160);

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
			ShadowMapPassDirectional(&mainLight);

			for (size_t i = 0; i < pointLightCount; i++) { ShadowMapPassOmni(&pointLights[i]); }
			for (size_t i = 0; i < spotLightCount; i++) { ShadowMapPassOmni(&spotLights[i]); }

			ForwardMainPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);
		}
		else
		{
			// deffered rendering
			DeferredMainPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);
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
		graphicUserInterface.EditRenderSettings(&forwardRender, &height, &splitScreenIsOn, &splitScreenType, 
												&gamma, &bloomThreshold, &brightness, &contrast, &saturation,
												&radiusAO, &biasAO);
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