#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>
#include <iostream>

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
			   GLfloat near, GLfloat far,
			   GLfloat red, GLfloat green, GLfloat blue, 
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat con, GLfloat lin, GLfloat exp);
	
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
				  GLuint diffuseIntensityLocation, GLuint positionLocation, 
				  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	void SetPosition(glm::vec3 pos);
	void SetLightRange(GLfloat range) {
		constant = 1.0f;
		linear = 4.5f / range;
		exponent = 75.0f / (range * range);
	}


	~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent, farPlane;

};