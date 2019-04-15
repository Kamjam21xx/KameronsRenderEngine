#pragma once

#include <GL\glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ShadowMap.h"

// plan to add light textures

class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight,
		  GLfloat red, GLfloat green, GLfloat blue, 
		  GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* GetShadowMap() { return shadowMap; }

	void SetAmbientIntensity(GLfloat newAmbientIntensity) {
		ambientIntensity = newAmbientIntensity;
	}
	void SetDiffuseIntensity(GLfloat newDiffuseIntensity) {
		diffuseIntensity = newDiffuseIntensity;
	}
	void SetLightColor(GLint red_8, GLint green_8, GLint blue_8) {
		color = glm::vec3(GLfloat(red_8 / 255.0f), GLfloat(green_8 / 255.0f), GLfloat(blue_8 / 255.0f));
	}
	void SetLightColor(GLfloat red, GLfloat green, GLfloat blue) {
		color = glm::vec3(red, green, blue);
	}
	glm::vec3 GetLightColor() {
		return color;
	}
	float GetLightColorRed() {
		return (float)color.r;
	}
	float GetLightColorGreen() {
		return (float)color.g;
	}
	float GetLightColorBlue() {
		return (float)color.b;
	}

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap *shadowMap;
};


