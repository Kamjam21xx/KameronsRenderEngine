#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLenum drawType);
	void CreateMeshNoTangents(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLenum drawType); // no tangent or bitangent

	void RenderMesh();
	void ClearMesh();

	~Mesh();


private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount; // stores number of indices to draw
};

