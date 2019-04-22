#ifndef COMMONVALS
#define COMMONVALS


#include "stb_image.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

/*
std::string reverseString(std::string sampleStr) {

	std::string returnStr = "";
	std::string::iterator element = sampleStr.end();
	
	for (int i = 0; element != sampleStr.begin(); --element, i++) {
		returnStr[i] = (*element);
	}

	return returnStr;
}
*/



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// using as a garbage collector for useful functions that need to be refactored elsewhere ATM

/*

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 normal;
	unsigned int in0;
	unsigned int in1;
	unsigned int in2;

	for (size_t i = 0; i < indiceCount; i += 3) {
		in0 = indices[i + 0] * vLength;
		in1 = indices[i + 1] * vLength;
		in2 = indices[i + 2] * vLength;

		v1 = glm::vec3(vertices[in1 + 0] - vertices[in0 + 0],
			vertices[in1 + 1] - vertices[in0 + 1],
			vertices[in1 + 2] - vertices[in0 + 2]
		);
		v2 = glm::vec3(vertices[in2 + 0] - vertices[in0 + 0],
			vertices[in2 + 1] - vertices[in0 + 1],
			vertices[in2 + 2] - vertices[in0 + 2]
		);

		normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset + 0] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
} // phong average

*/
#endif