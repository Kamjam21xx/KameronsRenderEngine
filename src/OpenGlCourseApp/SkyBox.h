#pragma once

#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"

class SkyBox
{
public:

	SkyBox();
	SkyBox(std::string fileLocation, const char *vertexShader, const char *fragmentShader);
	SkyBox(std::vector<std::string> faceLocations);

	void bindCubeMapTexture() const ;
	void unbinedCubeMapTexture() const ;
	void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) const ;

	// make functions to clear memory without destructing

	~SkyBox();

private:

	Mesh *skyMesh;
	Shader *skyShader;

	GLuint textureID;
	GLuint uniformProjection, uniformView;

	void LoadFromDirectory(std::string fileLocation, const char *vertexShader, const char *fragmentShader);
	void LoadTexturedCube(std::vector<std::string> faceLocations);
	void LoadShader(const char *vertexhader, const char *fragmentShader);

};