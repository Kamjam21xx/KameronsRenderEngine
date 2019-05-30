#include "ScreenQuad.h"



ScreenQuad::ScreenQuad()
{
	VAO = 0;
	VBO = 0;
}

void ScreenQuad::Init()
{
	float quadVertices[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
void ScreenQuad::Render()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
GLuint ScreenQuad::GetVAO()
{
	return VAO;
}
GLuint ScreenQuad::GetVBO()
{
	return VBO;
}


ScreenQuad::~ScreenQuad()
{
	if (VBO) 
	{
		glDeleteBuffers(1, &VBO);
	}
	VBO = 0;

	if (VAO) 
	{
		glDeleteVertexArrays(1, &VAO);
	}
	VAO = 0;
}
