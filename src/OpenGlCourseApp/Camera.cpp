#include "Camera.h"


Camera::Camera() {
	position = glm::vec3(0,0,0);
	worldUp = glm::vec3(0,1,0);
	yaw = 0;
	pitch = 0;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = 3;
	turnSpeed = 1;

	update();
}
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}
glm::vec3 Camera::getCameraDirection() {
	return glm::normalize(front);
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) { // matrix addition of vec3 values
	GLfloat velocity = moveSpeed * deltaTime; // note use of front to move with camera direction 
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		velocity *= 4.0f;
	}
	if (keys[GLFW_KEY_W]) { // forward
		position += front * velocity;
	}
	if (keys[GLFW_KEY_D]) { // right
		position += right * velocity;
	}
	if (keys[GLFW_KEY_A]) { // left
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S]) { // backward
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL]) { // backward
		position -= up * velocity;
	}
	if (keys[GLFW_KEY_SPACE]) { // backward
		position += up * velocity;
	}
}
void Camera::mouseControl(GLfloat xChange, GLfloat yChange) { // think in radians
	xChange = (xChange * turnSpeed);
	yChange = (yChange * turnSpeed);

	if (pitch < 0.0f) { 
		yaw += xChange;
	}
	if (pitch >= 0.0f) {
		yaw += xChange;
	}

	pitch += yChange;

	if (pitch > 59.0f) { // keep user from looking straight up.
		pitch = 59.0f;
	}
	if (pitch < -59.0f) { // keep user from looking straight down.
		pitch = -59.0f;
	}

	update();
}
glm::mat4 Camera::calculateViewMatrix() {

	return glm::lookAt(position, position + front, up);
}
void Camera::update() { // use mouseControl before keyControl in main
	std::thread updaterThread([&]() mutable -> void {
		front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front); // just getting direction, position irrelivant

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	});
	updaterThread.join();
}

Camera::~Camera()
{

}
