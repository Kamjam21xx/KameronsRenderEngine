#pragma once

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc\type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "CommonValues.h"

class Shader
{
public:
	Shader();
	
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetSpecularPowerLocation();
	GLuint GetEyePositionLocation();
	GLuint GetEyeDirectionLocation();
	GLuint GetOmniLightLocation();
	GLuint GetFarPlanelocation();

	void UseShader();
	void ClearShader();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	// possibly replace with predefined texture units 
	void SetTextureDiffuse(GLuint textureUnit); // keep texture unit for now
	void SetTextureSpecular(GLuint textureUnit); // keep texture unit for now
	void SetTextureMetal(GLuint textureUnit);
	void SetTextureNormal(GLuint textureUnit);
	void SetTextureHeight(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 *lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);
	void SetTextureSkyBox(GLuint textureUnit);
	
	~Shader();

private:

	int pointLightCount;
	int spotLightCount;
	GLuint shaderID, 
		uniformProjection, uniformModel, uniformView, 
		uniformEyePosition, uniformEyeDirection,
		uniformSpecularIntensity, uniformSpecularPower,
		uniformTextureDiffuse, uniformTextureSpecular, uniformTextureMetal, uniformTextureNormal, uniformTextureHeight,
		uniformTextureSkyBox,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	GLuint uniformLightMatrices[6];

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_SPOT_LIGHTS + MAX_POINT_LIGHTS];
	
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};

