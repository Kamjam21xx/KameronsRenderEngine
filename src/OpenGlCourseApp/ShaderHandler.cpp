#include "ShaderHandler.h"



ShaderHandler::ShaderHandler()
{

}

void ShaderHandler::LoadShaderSource() 
{
	vertexShader = ReadFile();
	fragmentShader = ReadFile();
	//geometryShader = ReadFile();
	//tesselationShader = ReadFile();
}

std::string ShaderHandler::GetVertexSource()						  
{
	return vertexShader;
}

std::string ShaderHandler::GetVertexSource() 
{
	return fragmentShader;
}

void ShaderHandler::ClearSource()				  
{
	vertexShader = "";
	fragmentShader = "";
	//geometryShader = "";
	//tesselationShader = "";
}

void ShaderHandler::ClearVertexSource() 
{
	vertexShader = "";
}

void ShaderHandler::ClearFragmentSource() 
{
	fragmentShader = "";
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

void ShaderHandler::SetPOMHeight()					  
{


}

void ShaderHandler::SetPOMSamples()					  
{


}

void ShaderHandler::SetGamma()                        
{

}

void ShaderHandler::SetDiffuseTU()
{


}

void ShaderHandler::SetSpecularTU()
{


}

void ShaderHandler::SetNormalTU()
{


}

void ShaderHandler::SetHeightTU()
{


}

void ShaderHandler::SetDShadowTU()
{


}

void ShaderHandler::SetSkyBoxTU()
{


}


void ShaderHandler::ToggleDiffuseTex()
{


}

void ShaderHandler::ToggleSpecularTex()
{


}

void ShaderHandler::ToggleNormalTex()
{


}

void ShaderHandler::TogglePOM()
{


}

void ShaderHandler::ToggleSkyBoxReflection()
{


}

void ShaderHandler::ToggleGamma()
{


}



ShaderHandler::~ShaderHandler()
{

}
