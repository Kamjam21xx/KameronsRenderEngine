#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLenum drawType);
	void CreateMeshNoTangents(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLenum drawType); 

	void RenderMesh() const ;
	void ClearMesh();

	~Mesh();


private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount; 
};

