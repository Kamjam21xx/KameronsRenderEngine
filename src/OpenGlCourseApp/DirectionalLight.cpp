#include "DirectionalLight.h"
#include <thread>



DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f); 
	lightProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f); // maybe make arrays of shadows with values altered by I & J
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
								   GLfloat red,        GLfloat green,      GLfloat blue, \
								   GLfloat aIntensity, GLfloat dIntensity,				 \
								   GLfloat xDir,       GLfloat yDir,       GLfloat zDir) 
								   : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity) {
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);
}


void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
								GLuint diffuseIntensityLocation, GLuint directionLocation) {
	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
glm::mat4 DirectionalLight::CalculateLightTransform() {
	glm::mat4 matrix4;
	std::thread calcLight([&]() mutable -> void {
		matrix4 = lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	});
	calcLight.join();
	return matrix4;
}

DirectionalLight::~DirectionalLight()
{
}
