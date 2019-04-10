#pragma once
#include <string>
#include <iostream>
#include <fstream>

class ShaderHandler									// rename ShaderSource
{													// possibly make a member of Shader class

													// UNDER CONSTRUCTION
														// source code editor for default shader

public:
	ShaderHandler();

	void LoadShaderSource();
	std::string GetVertexSource();
	std::string GetFragmentSource();
	//std::string GetGeometrySource();
	//std::string GetTesselationSource();
	void ClearSource();
	void ClearVertexSource();
	void ClearFragmentSource();

	std::string ReadFile();

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
	void SetPOMHeight();
	void SetPOMSamples();								// 2 values - min, max
	void SetGamma();	

	void SetDiffuseTU();
	void SetSpecularTU();
	void SetNormalTU();
	void SetHeightTU();
	void SetDShadowTU();
	void SetSkyBoxTU();

	void ToggleDiffuseTex();
	void ToggleSpecularTex();
	void ToggleNormalTex();
	void TogglePOM();
	void ToggleSkyBoxReflection();
	void ToggleGamma();

/*





	
	// set functions to elimate as many uniforms as possible/reasonable
*/
	~ShaderHandler();

private:
	std::string vertexShader;
	std::string fragmentShader;
	std::string geometryShader;
	std::string tesselationShader;

	// add checks for logic later

};

