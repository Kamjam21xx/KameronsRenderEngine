#include "GL_Window.h"
#include <thread>



GL_Window::GL_Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; ++i) {
		keys[i] = 0;
	}
}
GL_Window::GL_Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	sensitivity = 0.25f;

	for (size_t i = 0; i < 1024; ++i) {
		keys[i] = 0;
	}
}
GL_Window::GL_Window(GLint windowWidth, GLint windowHeight, GLfloat sensitivity)
{
	width = windowWidth;
	height = windowHeight;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	sensitivity = 0.25f;

	for (size_t i = 0; i < 1024; ++i) {
		keys[i] = 0;
	}
}

int GL_Window::Initialize() {
		if (!glfwInit()) {												// initialize GLFW
			glfwTerminate();
		}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);					// set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);		            // set minor version to 3

	// GLFW_OPENGL_COMPAT_PROFILE
	// GLFW_OPENGL_CORE_PROFILE

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// core profile does not allow backwards compatability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	mainWindow = glfwCreateWindow(width, height, "OPENGL WINDOW", NULL, NULL);
	
		if (!mainWindow) { printf("GLFW Window Creation Failure \n");
			glfwTerminate();
		}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // Get buffer size information
	glfwMakeContextCurrent(mainWindow);// set context for GLEW to use : telling glew to draw to our window 
	// handle key + mouse input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // change to make mouse appear id needed
	glewExperimental = GL_TRUE; // Allow modern extension features


		if (glewInit() != GLEW_OK) { printf("GLEW initialization failure \n"); // initialize glew
			glfwDestroyWindow(mainWindow);
			glfwTerminate();
		}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND); // equation ----- Cresult = Csource * Fsource + Cdestination * Fdestination
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glViewport(0, 0, bufferWidth, bufferHeight); // x start : y start : width : height

	glfwSetWindowUserPointer(mainWindow, this);
	
	return 0;
}
void GL_Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}
GLfloat GL_Window::getXChange() {
	GLfloat theChange = xChange * sensitivity;
	xChange = 0.0f;
	return theChange;
}
GLfloat GL_Window::getYChange() {
	GLfloat theChange = yChange * sensitivity;
	yChange = 0.0f;
	return theChange;
}
GLfloat GL_Window::getSensitivity()
{
	return sensitivity;
}
void GL_Window::setSensitivity(float newSensitivity)
{
	sensitivity = newSensitivity;
}

void GL_Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	GL_Window* theWindow = static_cast<GL_Window*>(glfwGetWindowUserPointer(window));
	std::thread threadZ([&]()mutable->void {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key >= 0 && key < 1024) {
			if (action == GLFW_PRESS) {
				theWindow->keys[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				theWindow->keys[key] = false;
			}
		}
	});
	threadZ.join();
}
void GL_Window::handleMouse(GLFWwindow *window, double xPos, double yPos) {
	GL_Window* theWindow = static_cast<GL_Window*>(glfwGetWindowUserPointer(window));
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		return;
	}
	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false; // or 0
	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // flip order for inverted mouse movement

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


GL_Window::~GL_Window()
{
}

