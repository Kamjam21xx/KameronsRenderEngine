#include "Mesh.h"



Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, 
					  unsigned int numOfVertices, unsigned int numOfIndices, 
					  GLenum drawType
					  ) 
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);																												 // now creating a vertex array on the graphics card in v-ram with the ID 
	glBindVertexArray(VAO);																												 // binding

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, drawType);
	
		glGenBuffers(1, &VBO);																				                          // creating a buffer object inside of the VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);																									// now working with with this VBO 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, drawType);	

		// vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 14, 0);
		// texture coordinate
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 14, (void*)(sizeof(vertices[0]) * 3));
		// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 14, (void*)(sizeof(vertices[0]) * 5));
		// tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 14, (void*)(sizeof(vertices[0]) * 8));
		// bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 14, (void*)(sizeof(vertices[0]) * 11));

	// ADD second UV coordinate to vertex for mapped lighting and more. 

	glBindBuffer(GL_ARRAY_BUFFER, 0);																								// unbind by binding gl array buffer from VBO while using VAO to 0
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);																									// unbind by binding gl element array buffer to 0 from IBO 

	glBindVertexArray(0);
}
void Mesh::CreateMeshNoTangents(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLenum drawType) {
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);																												 // now creating a vertex array on the graphics card in v-ram with the ID 
	glBindVertexArray(VAO);																												 // binding

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, drawType);

	glGenBuffers(1, &VBO);																				                          // creating a buffer object inside of the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);																									// now working with with this VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, drawType);
	// can also be --- sizeof(GLfloat x 9)

	// vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);

	// texture coordinate
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));

	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	// ADD second UV coordinate to vertex attrib array for mapped lighting and more. 



	// ADD second UV coordinate to vertex attrib array for mapped lighting and more. 

	glBindBuffer(GL_ARRAY_BUFFER, 0);																								// unbind by binding gl array buffer from VBO while using VAO to 0
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);																									// unbind by binding gl element array buffer to 0 from IBO 

	glBindVertexArray(0);
}


void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}


Mesh::~Mesh()
{
	// if created in a function and not as a pointer to the object, expect it to be destructed and not displayed on draw
	ClearMesh();
}
