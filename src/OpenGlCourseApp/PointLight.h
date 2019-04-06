#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

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
	
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
				  GLfloat diffuseIntensityLocation, GLfloat positionLocation, 
				  GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	void SetPosition(glm::vec3 pos);

	~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};