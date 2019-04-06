#pragma once

#include <vector>
#include <string>

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
	SkyBox(std::vector<std::string> faceLocations);

	void bindCubeMapTexture();
	void unbinedCubeMapTexture();
	void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~SkyBox();

private:
	Mesh *skyMesh;
	Shader *skyShader;

	GLuint textureID;
	GLuint uniformProjection, uniformView;
};

