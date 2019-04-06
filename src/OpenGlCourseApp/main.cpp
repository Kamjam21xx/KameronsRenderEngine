#pragma
#define STB_IMAGE_IMPLEMENTATION

// #include "stb_image.h"
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
//										 openGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//
#include "CommonValues.h"
//										 project 
#include "Mesh.h"
#include "Shader.h"
#include "GL_Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"
#include "SkyBox.h"
#include "Scene.h"

/* ----------- notes for "re-factoring" 

Check renderPass and restructure for speed as well as the ability to turn lights on and off conditionally
and or certain functionaity. 

*/
/*
	USE singular ray trace, to object, if mouse on true then highlight around edge.

	if clicked use c++ hand written side camera move to object 
		use time
		function
		reduce to range between 0-1
		translate

	setup time inside shader - maybe

	draw simple diagrams and notes up and code tomorrow
*/
/*
	make a "global" object to encapsulate certain things, called the "Scene" 

	so references are "scene.lights[]" or something like that.
	make sure it can be emptied without being deleted for future flexibility without refactoring.
	create functions for resizing all containers that are automatically invoked in some cases
*/
/*
					note to self when modeling. create planes before resorting to boolean exclude include etc
					start bools from bottom, in counter clockwise, thats best for c4d algos
*/

// TEMP TEMP TEMP TEMP
unsigned int framebuffer;
unsigned int texColorBuffer;	
unsigned int rbo;
// // // // // // // // // //


const float toRadians = 3.14159265 / 180.0;

GL_Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
// instantiate global vectors for frame buffers and make frame buffer class

Shader directionalShadowShader;
Shader omniShadowShader;
Shader outlineShader;
Shader framebuffershader;

Camera camera;

Model church;

Texture space1Texture;
Texture space2Texture;
Texture gridTexture;

Material shineMaterial;
Material dullMaterial;

SkyBox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

// main shader programs
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";
static const char* olvShader = "Shaders/olfshader.vert";
static const char* olfShader = "Shaders/olfshader.frag";
static const char* outlineVShader = "Shaders/outline.vert";
static const char* outlineFShader = "Shaders/outline.frag";
static const char* FBVShader = "Shaders/framebuffershader.vert";
static const char* FBFShader = "Shaders/framebuffershader.frag";

GLfloat spin;
	/*
	for (int i = 0; i != 5; ++i) {
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.4 * (float)i * 1.0f, 0.4 * (float)i * 1.0f, 1.0 * (float)i * 1.0f));
		model = glm::translate(model, glm::vec3((float) -i, (float) i, (float) -i));

		glm::rot
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		space1Texture.UseTexture();

		meshList[0]->RenderMesh();
	}
	*/
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

	/*
	Shader *shader2 = new Shader();
	shader1->CreateFromFiles(vShader, fShader2);
	shaderList.push_back(*shader2);
	*/
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
	// glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	// GL_FALSE means we do not want to transpose the matrix in this function


	for (auto element = mainScene.objects.begin() ; element != mainScene.objects.end(); ++element) {
		glm::mat4 model = glm::mat4(1.0);
		// model = glm::mat4(1.0f);
		model = glm::rotate(model, spin * toRadians, glm::vec3(0.0, 1.0, 0.0));
		// model = glm::translate(model, glm::vec3(2.0, -2.85, 3.0));
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
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate(); // seems wasteful

	

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
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
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void RenderPass(glm::mat4 projectionMatrix, 
				glm::mat4 viewMatrix, 
				unsigned int* pointLightCount, 
				unsigned int* spotLightCount, 
				DirectionalLight* mainLight, 
				PointLight *pointLights, 
				SpotLight *spotLights) {

	glViewport(0, 0, 3840, 2160);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	//glViewport(0, 0, 3840, 2160);
	
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
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
	skybox.bindCubeMapTexture(); // bind skybox cube map to texture unit 6 for sampling of reflections
	RenderScene();	
	skybox.unbinedCubeMapTexture();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	skybox.DrawSkyBox(viewMatrix, projectionMatrix);

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void CreateLights(PointLight &pointLightsR, 
				  SpotLight &spotLightsR, 
				  unsigned int *pointLightCount, 
				  unsigned int *spotLightCount) {

	// make plain text loader for lights, add to scene.

	PointLight *pointLights = &pointLightsR; // change later
	SpotLight *spotLights = &spotLightsR;

	
	pointLights[0] = PointLight(2048, 2048,
		0.01f, 100.0f,
		7.0f, 7.0f, 7.0f,
		0.1f, 0.8f,
		1.0f, 3.50f, 1.0f,
		0.8f, 0.01f, 0.00001f);
	(*pointLightCount)++;
/*
	pointLights[1] = PointLight(2048, 2048,
		0.01f, 100.0f,
		7.0f, 7.0f, 7.0f,
		0.1f, 0.8f,
		-3.0f, 5.50f, 4.0f,
		0.8f, 0.01f, 0.00001f);
	(*pointLightCount)++;

	pointLights[2] = PointLight(2048, 2048,
		0.01f, 100.0f,
		1.0f, 1.0f, 0.0f,
		0.00f, 0.4f,
		4.0f, 8.0f, 4.0f,
		0.35f, 0.1f, 0.05f);
	(*pointLightCount)++;

	spotLights[0] = SpotLight(2048, 2048,
		0.1f, 100.0f,
		0.005f, 0.005f, 0.005f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0001f, 0.0f, 0.0f,
		20.0f); 
	(*spotLightCount)++;
*/
}



int main() 
{	
	mainWindow = GL_Window(3840, 2160);
	mainWindow.Initialize();

	glViewport(0, 0, 3840, 2160);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	


	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	//mainWindow.swapBuffers();

	glEnable(GL_DEBUG_OUTPUT);

//<>=========================================================================================================<>

	DirectionalLight mainLight;
	std::vector<std::string> skyboxFaces;
	glm::mat4 projection;

	std::thread threadY([&]()mutable->void{
		camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90.0f, 0, 3.0f, 0.5f);
		projection = glm::perspective(glm::radians(52.0f), mainWindow.getAspectRatio(), 0.1f, 100.0f);

		// space1Texture = Texture("Textures/gea.png");
		// space2Texture = Texture("Textures/space2.png");
		// gridTexture = Texture("Textures/gridtexture.png");
	});
	std::thread threadX([&]() mutable -> void {
		shineMaterial = Material(4.0f, 32.0f);
		dullMaterial = Material(0.5f, 50.0f);

		skyboxFaces.push_back("Textures/skybox/right.jpg"); // take note of the file type
		
		skyboxFaces.push_back("Textures/skybox/left.jpg");
		
		skyboxFaces.push_back("Textures/skybox/top.jpg");
		
		skyboxFaces.push_back("Textures/skybox/bot.jpg");
		
		skyboxFaces.push_back("Textures/skybox/back.jpg");
		
		skyboxFaces.push_back("Textures/skybox/front.jpg");
	});

	 // add file loc pass overload


	GLfloat now;

	mainLight = DirectionalLight(256, 256,
								 0.01f, 0.01f, 0.01f,
								 0.25f, 0.8f,
								 -5.0f, -10.0f, -10.0f);

	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
	unsigned int spotLightCount = 0;	
	unsigned int pointLightCount = 0;
	CreateLights(*pointLights, *spotLights, &pointLightCount, &spotLightCount);
	CreateShaders();

	threadX.join();

	skybox = SkyBox(skyboxFaces);
	  
	threadY.join();

    mainScene.load();

	//space1Texture.LoadTextureA();
	//space2Texture.LoadTextureA();
	//gridTexture.LoadTextureA();

//<>=========================================================================================================<>

	std::thread threadB;

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_FRAMEBUFFER_SRGB);

//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>

	/*                FRAME BUFFERS                     */

	/*
	// make frame buffer
	unsigned int fbo, texture;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) {
		// check frame buffer and error report
		printf("ERROR BINDING FRAMEBUFFER");
		goto skipFBO;
	}
	// generate texture for frame buffer
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
				 mainWindow.getWindowWidth(), mainWindow.getWindowHeight(),
				 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// bind texture to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


	// make render buffer         -           write only, fast writes, native gl format
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mainWindow.getWindowWidth(), mainWindow.getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


	*/
	//<>=======<>
/*

	// make frame buffer
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture for frame buffer
	
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mainWindow.getWindowWidth(), mainWindow.getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// make render buffer

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mainWindow.getWindowWidth(), mainWindow.getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) {
		// check frame buffer and error report
		printf("ERROR BINDING FRAMEBUFFER");
		goto skipFBO;
	}

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// quad 
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	// quad object
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		framebuffershader.SetTextureDiffuse(8);


skipFBO : 
		*/
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>
//<>=========================================================================================================<>


	while (!mainWindow.getShouldClose()) {
		

		std::thread threadB(&GL_Window::swapBuffers, mainWindow);

		std::thread threadA([&]() mutable -> void {
			now = glfwGetTime();

			deltaTime = now - lastTime;

			lastTime = now;
			
		}
		);

		spin += 0.1f;
		if (spin >= 360.00f) { spin = 0.0f; }

		glfwPollEvents();		

		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange()); // calls threaded func "update" - use before "keyControl"
		threadA.join();
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		threadB.join(); // buffers swapped by this point

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glStencilMask(0x00);

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++) {
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++) {
			OmniShadowMapPass(&spotLights[i]);
		}

		/*
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		*/

		/*

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		*/
		RenderPass(projection, camera.calculateViewMatrix(), &pointLightCount, &spotLightCount, &mainLight, pointLights, spotLights);






		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, 3840, 2160);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		//glDisable(GL_DEPTH_TEST);

		//glActiveTexture(GL_TEXTURE5);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		//framebuffershader.UseShader();

		//framebuffershader.SetTextureDiffuse(5);

		//glBindVertexArray(quadVAO);

		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		//glDrawArrays(GL_TRIANGLES, 0, 6);



		/*
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outlineShader.UseShader();

			uniformModel = outlineShader.GetModelLocation();
			uniformProjection = outlineShader.GetProjectionLocation();
			uniformView = outlineShader.GetViewLocation();
			uniformEyePosition = outlineShader.GetEyePositionLocation();

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			outlineShader.SetTextureDiffuse(1);

		outlineShader.Validate();

		RenderSceneStencil();

		glStencilMask(0xFF);

		glEnable(GL_DEPTH_TEST);
		*/
		
		glUseProgram(0); // use no shader

	}

	return 0;
}



/*

DEFAULT IMPLICIT CONSTRUCTOR DOESNT INITIALIZE VALUES ON glm::mat4 
either use the explicit constructor -- or -- a compiler for older versions of c++ 
// glm 4x4 matrice

			-------->			glm::mat4 model = glm::mat4(1.0);        <--------

*/

/*
void CreateObjects() {
	
	unsigned int indices[] = {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2
	};
	GLfloat vertices[] = { // x - y - z     matrice 
		//    x      y     z       u    v        nx    ny    nz                  vertices - UV tex coordinates - normals
			-1.0, -1.0, -0.6,      0.0, 0.0,	0.0f, 0.0f, 0.0f, 
			0.0 , -1.0, 1.0,      2.0, 0.0,		0.0f, 0.0f, 0.0f,
			1.0 , -1.0, -0.6,      4.0, 0.0,	0.0f, 0.0f, 0.0f,
			0.0 , 1.0 , 0.0,      2.0, 4.0,		0.0f, 0.0f, 0.0f
	};

	GLfloat floorVertices[] = {
			-10.0f, 0.0f, -10.f,		0.0f, 0.0f,				0.0f, -1.0f, 0.0f,
			10.0f, 0.0f, -10.0f,		10.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			-10.0f, 0.0f, 10.0f,		0.0f, 10.0f,			0.0f, -1.0f, 0.0f,
			10.0f, 0.0f, 10.0f,			10.0f, 10.0f,			0.0f, -1.0f, 0.0f
	};
	unsigned int floorIndices[] = {
			0, 2, 1,
			1, 2, 3
	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMeshNoTangents(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMeshNoTangents(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	
	Mesh *obj3 = new Mesh();
	obj3->CreateMeshNoTangents(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}
*/