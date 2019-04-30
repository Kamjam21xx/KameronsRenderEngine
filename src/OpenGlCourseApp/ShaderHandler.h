
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class ShaderHandler									// rename ShaderSource
{													// possibly make a member of Shader class

													// UNDER CONSTRUCTION
														// source code editor for default shader


	// could use reverse finds, but im keeping them normal finds for readability. 
	// keepin it simple

	// use preprocessor statements and SCRAP EVERYTHING (most everything)
	// defines elif ifndef etc

public:
	ShaderHandler();

	void LoadShaderSource(std::string vertShaderSource, std::string fragShaderSource);

	void ClearAllSource();
	void ClearVertexSource();
	void ClearFragmentSource();
	void ClearGeometrySource();
	void ClearTesselationSource();

	const std::string GetVertexSourceStr();
	const std::string GetFragmentSourceStr();
	const std::string GetGeometrySourceStr();
	const std::string GetTesselationSourceStr();

	const char* GetVertexSourcePtr();
	const char* GetFragmentSourcePtr();
	const char* GetGeometrySourcePtr();
	const char* GetTesselationSourcePtr();

	std::string ReadFile(std::string fileLocation);


	void SetDLight();
	void SetDLightDirection();
	void SetDLightColor();
	void SetDLightAmbientIntensity();
	void SetDLightDiffuseIntensity();

	void SetPLightCount();
	void SetSLightEdge();

	void SetSpotLightCount();
												// all variables set will be static _ give array of lights
	void SetLightsAuto();					
	void SetLightColor();					
	void SetLightDistance();								// set exponent, linear, and constant via same math as light distance func
	void SetLightDiffuseIntensity();
	void SetLightAmbientIntensity();

	void SetPLightBias();
	void SetPLightSamples();
	void SetPLightSampleRadiusDivisor();					// default is 85.0f
	void SetPLightSampleRadiusBase();					 // default is 1.0f
	void SetDLightBiasMin();
	void SetDLightBiasMaxMultiplier();
													// edit default shader to add DLight sample setter();
	void SetPOMHeight(GLfloat heightPOM);
	void SetPOMSamples(GLuint newSampleCount);								// 2 values - min, max
	void SetGamma(GLfloat newGamma);	

	void SetDiffuseTU(GLuint textureUnit);
	void SetSpecularTU(GLuint textureUnit);
	void SetNormalTU(GLuint textureUnit);
	void SetHeightTU(GLuint textureUnit);
	void SetDShadowTU(GLuint textureUnit);
	void SetSkyTU(GLuint textureUnit);

	void ToggleDiffuseTex();
	void ToggleSpecularTex();
	void ToggleNormalTex();
	void TogglePOM();
	void ToggleSkyReflection();
	void ToggleGamma();

	GLuint GetDiffuseTU() 
	{ 
		return diffuseTU; 
	}
	GLuint GetSpecularTU() 
	{ 
		return specularTU; 
	}
	GLuint GetNormalTU() 
	{
		return normalTU; 
	}
	GLuint GetHeightTU() 
	{ 
		return heightTU; 
	}
	GLuint GetDirectionalShadowTU()
	{ 
		return directionalShadowTU; 
	}
	GLuint GetSkyBoxTU() 
	{ 
		return skyboxTU; 
	}
	GLuint GetScreenSpaceTU()
	{
		return screenSpaceTU;
	}
	GLuint GetScreenSpaceTwoTU()
	{
		return screenSpaceTwoTU;
	}

/*





	
	// set functions to elimate as many uniforms as possible/reasonable
*/
	// setup material class so the two can work together


	~ShaderHandler();

private:

	std::string vertexShader;
	std::string fragmentShader;
	std::string geometryShader;
	std::string tesselationShader;

	GLuint diffuseTU,
		   specularTU,
		   normalTU,
		   heightTU,
		   directionalShadowTU,
		   skyboxTU,
		   screenSpaceTU,
		   screenSpaceTwoTU;

	bool toggleDiffuseTex,
		 toggleSpecularTex,
		 toggleNormalTex,
		 togglePOM,
		 toggleSkyBoxReflection,
		 toggleGamma,
		 toggleScreenSpaceTex,
		 toggleScreenSpaceTexTwo;
	
	GLfloat gamma, 
			heightPOM;

	GLuint samplesPOM

	// add checks for logic later

};