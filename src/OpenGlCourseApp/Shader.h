#pragma once

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\matrix_transform.hpp>

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
	void Validate() const ;

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() const ;
	GLuint GetInverseProjectionLocation() const ;
	GLuint GetModelLocation() const ;
	GLuint GetViewLocation() const ;
	GLuint GetInverseViewLocation() const;
	GLuint GetAmbientIntensityLocation() const ;
	GLuint GetAmbientColorLocation() const ;
	GLuint GetDiffuseIntensityLocation() const ;
	GLuint GetDirectionLocation() const ;
	GLuint GetSpecularIntensityLocation() const ;
	GLuint GetSpecularPowerLocation() const ;
	GLuint GetEyePositionLocation() const ;
	GLuint GetEyeDirectionLocation() const ;
	GLuint GetOmniLightLocation() const ;
	GLuint GetFarPlanelocation() const ;
	GLuint GetSplitScreenIsOnLocation() const ;
	GLuint GetSplitScreenTypeLocation() const ;
	GLuint GetTextureScreenSpaceLocation() const ;
	GLuint GetTextureScreenSpaceTwoLocation() const;
	GLuint GetTextureScreenSpaceThreeLocation() const;
	GLuint GetTextureDepthLocation() const;
	GLuint GetTextureNoiseSSAOLocation() const;
	GLuint GetGammaLocation() const ;
	GLuint GetBloomThresholdLocation() const ;
	GLuint GetBloomStrengthLocation() const;
	GLuint GetBlurDirectionLocation() const ;
	GLuint GetBrightnessLocation() const;
	GLuint GetContrastLocation() const;
	GLuint GetSaturationLocation() const;
	GLuint GetHeightPOMLocation() const;
	GLuint GetRandomSamplesSSAOLocation(int index) const;
	GLuint GetTextureAOLocation() const;
	GLuint GetAORadiusLocation() const;
	GLuint GetAOBiasLocation() const;

	void UseShader();
	void ClearShader();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTextureDiffuse(GLuint textureUnit);
	void SetTextureSpecular(GLuint textureUnit);
	void SetTextureMetal(GLuint textureUnit);
	void SetTextureNormal(GLuint textureUnit);
	void SetTextureHeight(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetInverseProjection(glm::mat4 *inverseProjectionMatrix);
	void SetInverseView(glm::mat4 *inverseViewMatrix);
	void SetDirectionalLightTransform(glm::mat4 *lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);//-------------- copies vector - passed by value
	void SetLightMatrices(std::vector<glm::mat4> *lightMatrices);
	void SetLightMatrices(glm::vec3 *position, glm::mat4 *lightProjection);
	void SetTextureSkyBox(GLuint textureUnit);
	void SetSplitScreenIsOn(GLboolean splitScreenIsOn);
	void SetSplitScreenIsOn(GLboolean *splitScreenIsOn);
	void SetSplitScreenType(GLuint splitScreenType);
	void SetSplitScreenType(GLuint *splitScreenType);
	void SetTextureScreenSpace(GLuint textureUnit);
	void SetTextureScreenSpaceTwo(GLuint textureUnit);
	void SetTextureScreenSpaceThree(GLuint textureUnit);
	void SetTextureDepth(GLuint textureUnit);
	void SetTextureNoiseSSAO(GLuint textureUnit);
	void SetGamma(GLfloat gammaLevel);
	void SetGamma(GLfloat *gammaLevel);
	void SetBloomThreshold(GLfloat bloomThreshold);
	void SetBloomThreshold(GLfloat *bloomThreshold);
	void SetBloomStrength(GLfloat bloomStrength);
	void SetBloomStrength(GLfloat *bloomStrength);
	void SetBlurDirection(glm::vec2 blurDirection);
	void SetBlurDirection(glm::vec2 *blurDirection);
	void SetBrightness(GLfloat brightness);
	void SetBrightness(GLfloat *brightness);
	void SetContrast(GLfloat contrast);
	void SetContrast(GLfloat *contrast);
	void SetSaturation(GLfloat saturation);
	void SetSaturation(GLfloat *saturation);
	void SetHeightPOM(GLfloat height);
	void SetHeightPOM(GLfloat *height);
	void SetRandomSamplesSSAO(std::vector<glm::vec3> randomSamples);//-------------- copies vector - passed by value
	void SetRandomSamplesSSAO(std::vector<glm::vec3> *randomSamples);
	void SetTextureAO(GLuint textureUnit);
	void SetAmbientOcclusionRadius(GLfloat radius);
	void SetAmbientOcclusionRadius(GLfloat *radius);
	void SetAmbientOcclusionBias(GLfloat bias);
	void SetAmbientOcclusionBias(GLfloat *bias);

	~Shader();

private:

	int pointLightCount;
	int spotLightCount;
	GLuint shaderID, 
		   uniformProjection, uniformInverseProjection, uniformModel, uniformView, uniformInverseView, 
		   uniformEyePosition, uniformEyeDirection,
		   uniformSpecularIntensity, uniformSpecularPower,
		   uniformTextureDiffuse, uniformTextureSpecular, uniformTextureMetal, uniformTextureNormal, uniformTextureHeight,
		   uniformTextureSkyBox,
		   uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		   uniformOmniLightPos, uniformFarPlane, uniformSplitScreenIsOn, uniformSplitScreenType,
		   uniformTextureScreenSpace, uniformTextureScreenSpaceTwo, uniformTextureScreenSpaceThree,
		   uniformTextureDepth, uniformTextureNoiseSSAO, uniformTextureAO,
		   uniformGamma, uniformBloomThreshold, uniformBloomStrength, uniformBlurDirection,
		   uniformBrightness, uniformContrast, uniformSaturation,
		   uniformHeightPOM,
		   uniformRadiusAO, uniformBiasAO;

	GLuint uniformLightMatrices[6];

	GLuint uniformRandomSamplesSSAO[64];

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

