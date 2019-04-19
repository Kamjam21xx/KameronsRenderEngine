#pragma once
#include <GL\glew.h>

#include <vector>

#include "Texture.h" 

// throw in vector of texture*
// refactor material class properly to contain the correct objects and functions

class Material
{
public:

	Material();
	Material(GLfloat sIntensity, GLfloat specularP);

	void UseMaterial(GLuint specularIntensityLocation, GLuint specularPowerLocation) const ;
	
	/*
	void incIntensity() { specularIntensity += 0.05f; };
	void incPower() { specularPower += 0.05f; };
	void decIntensity() { specularIntensity -= 0.05f; };
	void decPower() { specularPower -= 0.05f; };
	*/

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat specularPower;



};

