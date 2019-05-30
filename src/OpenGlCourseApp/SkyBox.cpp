#include "SkyBox.h"



SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::string fileLocation, const char *vertexShader, const char *fragmentShader) // directory to folder 
{
	LoadFromDirectory(&fileLocation, vertexShader, fragmentShader, nullptr, nullptr);
}
SkyBox::SkyBox(std::string fileLocation, const char *vertexShader, const char *fragmentShader, const char *vertexShaderD, const char *fragmentShaderD) // directory to folder 
{
	LoadFromDirectory(&fileLocation, vertexShader, fragmentShader, vertexShaderD, fragmentShaderD);
}

SkyBox::SkyBox(std::vector<std::string> faceLocations)
{
	const char* vert = "Shaders/skybox.vert";
	const char* frag = "Shaders/skybox.frag";
	const char* vertD = "Shaders/skyboxDeferred.vert";
	const char* fragD = "shaders/skyboxDeferred.frag";

	LoadShader(vert, frag);
	LoadTexturedCube(&faceLocations);
}
void SkyBox::LoadFromDirectory(std::string *fileLocation, const char *vertexShader, const char *fragmentShader) // directory to folder 
{
	LoadFromDirectory(fileLocation, vertexShader, fragmentShader, nullptr, nullptr);
}
void SkyBox::LoadFromDirectory(std::string *fileLocation, const char *vertexShader, const char *fragmentShader, const char *vertexShaderD, const char *fragmentShaderD) // directory to folder 
{
	std::vector<std::string> faceLocations;
	faceLocations.push_back((*fileLocation) + "/right.jpg");
	faceLocations.push_back((*fileLocation) + "/left.jpg");	
	faceLocations.push_back((*fileLocation) + "/top.jpg");
	faceLocations.push_back((*fileLocation) + "/bot.jpg");
	faceLocations.push_back((*fileLocation) + "/back.jpg");
	faceLocations.push_back((*fileLocation) + "/front.jpg");
	
	if (vertexShader != nullptr)
	{
		LoadShader(vertexShader, fragmentShader);
	}
	if (vertexShaderD != nullptr)
	{
		LoadShaderDeferred(vertexShaderD, fragmentShaderD);
	}

	LoadTexturedCube(&faceLocations);
}

void SkyBox::LoadShader(const char *vertexShader, const char *fragmentShader)
{
	skyShader = new Shader();
	skyShader->CreateFromFiles(vertexShader, fragmentShader);

	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();
}

void SkyBox::LoadShaderDeferred(const char *vertexShader, const char *fragmentShader)
{
	skyShaderDeferred = new Shader();
	skyShaderDeferred->CreateFromFiles(vertexShader, fragmentShader);

	uniformProjectionDeferred = skyShaderDeferred->GetProjectionLocation();
	uniformViewDeferred = skyShaderDeferred->GetViewLocation();
}

void SkyBox::LoadTexturedCube(std::vector<std::string> *faceLocations) 
{
	// Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, bitDepth;
	for (size_t i = 0; i < 6; ++i) 
	{
		unsigned char *texData = stbi_load((*faceLocations)[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData) {
			printf("Failed to find: %s \n", (*faceLocations)[i].c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Cube Mesh
	unsigned int skyBoxIndices[]{
	0, 1, 2,
	2, 1, 3,

	2, 3, 5,
	5, 3, 7,

	5, 7, 4,
	4, 7, 6,

	4, 6, 0,
	0, 6, 1,

	4, 0, 5,
	5, 0, 2,

	1, 6, 3,
	3, 6, 7,
	};
	float skyBoxVertices[]{
		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	// Mesh Setup
	skyMesh = new Mesh();
	skyMesh->CreateMeshNoTangents(skyBoxVertices, skyBoxIndices, 64, 36, GL_STATIC_DRAW);
}

void SkyBox::bindCubeMapTexture() const 
{
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void SkyBox::DrawSkyBox(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix, GLfloat *gamma, GLfloat *bloomThreshold, GLfloat *brightness, GLfloat *contrast, GLfloat *saturation) const // add brightness contrast saturation
{
	GLfloat viewMatrixArray[16] =
	{
	(*viewMatrix)[0][0],  (*viewMatrix)[0][1],  (*viewMatrix)[0][2],  0.0f,
	(*viewMatrix)[1][0],  (*viewMatrix)[1][1],  (*viewMatrix)[1][2],  0.0f,
	(*viewMatrix)[2][0],  (*viewMatrix)[2][1],  (*viewMatrix)[2][2],  0.0f,
	0.0f,				  0.0f,				    0.0f,				  0.0f
	};

	// glDepthMask(GL_FALSE);

	skyShader->UseShader();
	skyShader->SetGamma(*gamma);
	skyShader->SetBloomThreshold(*bloomThreshold);
	skyShader->SetBrightness(*brightness);
	skyShader->SetContrast(*contrast);
	skyShader->SetSaturation(*saturation);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, viewMatrixArray);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyShader->Validate();
	
	skyMesh->RenderMesh();

	// glDepthMask(GL_TRUE);
}
void SkyBox::DrawSkyBoxDeferred(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix, GLfloat *gamma, GLfloat *bloomThreshold, GLfloat *brightness, GLfloat *contrast, GLfloat *saturation) const
{
	GLfloat viewMatrixArray[16] =
	{
	(*viewMatrix)[0][0],  (*viewMatrix)[0][1],  (*viewMatrix)[0][2],  0.0f,
	(*viewMatrix)[1][0],  (*viewMatrix)[1][1],  (*viewMatrix)[1][2],  0.0f,
	(*viewMatrix)[2][0],  (*viewMatrix)[2][1],  (*viewMatrix)[2][2],  0.0f,
	0.0f,				  0.0f,				    0.0f,				  0.0f
	};

	// glDepthMask(GL_FALSE);

	skyShaderDeferred->UseShader();
	skyShaderDeferred->SetGamma(*gamma);
	skyShaderDeferred->SetBloomThreshold(*bloomThreshold);
	skyShaderDeferred->SetBrightness(*brightness);
	skyShaderDeferred->SetContrast(*contrast);
	skyShaderDeferred->SetSaturation(*saturation);

	glUniformMatrix4fv(uniformProjectionDeferred, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
	glUniformMatrix4fv(uniformViewDeferred, 1, GL_FALSE, viewMatrixArray);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyShaderDeferred->Validate();

	skyMesh->RenderMesh();

	// glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{
	// delete[] skyMesh;
	// delete[] skyShader;
}
