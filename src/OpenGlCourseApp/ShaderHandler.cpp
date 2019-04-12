
#include "ShaderHandler.h"



ShaderHandler::ShaderHandler()
{
	toggleDiffuseTex = true;
	toggleSpecularTex = true;
	toggleNormalTex = true;
	togglePOM = true;
	toggleSkyBoxReflection = true;
	toggleGamma = true;

	diffuseTU = 1;
	specularTU = 4;
	normalTU = 5;
	heightTU = 14;
	directionalShadowTU = 2;
	skyboxTU = 6;
}

void ShaderHandler::LoadShaderSource() 
{
	vertexShader = ReadFile(/*xXxXxXxXX*/);
	fragmentShader = ReadFile(/*xXxXxXxXX*/);
	//geometryShader = ReadFile();
	//tesselationShader = ReadFile();
}

void ShaderHandler::ClearAllSource()				  
{
	vertexShader.clear();
	fragmentShader.clear();
	//geometryShader.clear();
	//tesselationShader.clear();
}
void ShaderHandler::ClearVertexSource()	
{
	vertexShader.clear();
}
void ShaderHandler::ClearFragmentSource()
{
	fragmentShader.clear();
}
void ShaderHandler::ClearGeometrySource()
{
	geometryShader.clear();
}
void ShaderHandler::ClearTesselationSource()
{
	tesselationShader.clear();
}

const std::string ShaderHandler::GetVertexSourceStr()						
{
	return vertexShader;
}
const std::string ShaderHandler::GetFragmentSourceStr()	
{
	return fragmentShader;
}
const std::string ShaderHandler::GetGeometrySourceStr()	
{
	return geometryShader;
}
const std::string ShaderHandler::GetTesselationSourceStr()
{
	return tesselationShader;
}

const char* ShaderHandler::GetVertexSourcePtr()	
{
	const char* shaderSrcPtr = vertexShader.c_str();
	return shaderSrcPtr;
}
const char* ShaderHandler::GetFragmentSourcePtr()
{
	const char* shaderSrcPtr = fragmentShader.c_str();
	return shaderSrcPtr;
}
const char* ShaderHandler::GetGeometrySourcePtr()
{
	const char* shaderSrcPtr = geometryShader.c_str();
	return shaderSrcPtr;
}
const char* ShaderHandler::GetTesselationSourcePtr()
{
	const char* shaderSrcPtr = tesselationShader.c_str();
	return shaderSrcPtr;
}

std::string ReadFile(std::string fileLocation) 
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.");
		content = "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	return content;
}


void ShaderHandler::SetDLight()						  
{


}
void ShaderHandler::SetDLightDirection()			  
{


}
void ShaderHandler::SetDLightColor()				  
{


}
void ShaderHandler::SetDLightAmbientIntensity()		  
{


}
void ShaderHandler::SetDLightDiffuseIntensity()		  
{


}
void ShaderHandler::SetPLightCount()				  
{


}
void ShaderHandler::SetSLightEdge()					  
{


}
void ShaderHandler::SetSpotLightCount()				  
{


}
void ShaderHandler::SetLightsAuto()					  
{


}
void ShaderHandler::SetLightColor()					  
{


}
void ShaderHandler::SetLightDistance()				  
{


}
void ShaderHandler::SetLightDiffuseIntensity()		  
{


}
void ShaderHandler::SetLightAmbientIntensity()		  
{


}
void ShaderHandler::SetPLightBias()					  
{


}
void ShaderHandler::SetPLightSamples()				  
{


}
void ShaderHandler::SetPLightSampleRadiusDivisor()	  
{


}
void ShaderHandler::SetPLightSampleRadiusBase()		  
{


}
void ShaderHandler::SetDLightBiasMin()				  
{


}
void ShaderHandler::SetDLightBiasMaxMultiplier()	  
{


}
void ShaderHandler::SetPOMHeight(GLfloat newHeightPOM)					  
{
	std::string floatValue = std::to_string(heightPOM);
	std::string currentHeight = "float gamma = " + floatValue;

	auto begin = fragmentShader.find(currentHeight);
	begin = fragmentShader.find(floatValue, begin);
	auto end = begin + floatValue.length();

	std::string newFloatValue = std::to_string(newHeightPOM);
	fragmentShader.replace(begin, end, newFloatValue);
}
void ShaderHandler::SetPOMSamples(GLuint newSampleCount)
{


}
void ShaderHandler::SetGamma(float newGamma)
{

}

void ShaderHandler::SetDiffuseTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + diffuseTU;
	
	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(diffuseTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	diffuseTU = textureUnit;
}
void ShaderHandler::SetSpecularTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + specularTU;

	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(specularTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	specularTU = textureUnit;
}
void ShaderHandler::SetNormalTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + normalTU;

	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(specularTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	normalTU = textureUnit;
}
void ShaderHandler::SetHeightTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + heightTU;

	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(heightTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	heightTU = textureUnit;
}
void ShaderHandler::SetDShadowTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + directionalShadowTU;

	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(heightTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	directionalShadowTU = textureUnit;
}
void ShaderHandler::SetSkyTU(GLuint textureUnit)
{
	std::string currentBinding = "binding = " + skyboxTU;

	auto begin = fragmentShader.find(currentBinding);
	auto end = fragmentShader.find(skyboxTU, begin);
	fragmentShader.replace(end, end, (const char*)textureUnit);

	skyboxTU = textureUnit;
}


void ShaderHandler::ToggleDiffuseTex()
{
	toggleDiffuseTex = !toggleDiffuseTex;

	// remove tex
}
void ShaderHandler::ToggleSpecularTex()
{
	toggleSpecularTex = !toggleSpecularTex;

	// remove tex
}
void ShaderHandler::ToggleNormalTex()
{
	toggleNormalTex = !toggleNormalTex;

	// remove tex and TBN, switch to inverse transpose normal
}
void ShaderHandler::TogglePOM()
{
	togglePOM = !togglePOM;

	//remove tex and function
}
void ShaderHandler::ToggleSkyReflection()
{
	toggleSkyBoxReflection = !toggleSkyBoxReflection;

	// remove reflection calculation and function
}
void ShaderHandler::ToggleGamma()
{
	toggleGamma = !toggleGamma;

	// remove gamma value and lines
}



ShaderHandler::~ShaderHandler()
{

}

