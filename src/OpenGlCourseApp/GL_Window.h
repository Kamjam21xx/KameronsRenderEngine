#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GL_Window
{
public:
	GLFWwindow *mainWindow;

	GL_Window();
	GL_Window(GLint windowWidth, GLint windowHeight);	
	GL_Window(GLint windowWidth, GLint windowHeight, GLfloat sensitivity);
	int Initialize();
	GLfloat getAspectRatio() { return ( (GLfloat)bufferWidth / (GLfloat)bufferHeight ); }
	GLint getWindowWidth() { return width; }
	GLint getWindowHeight() { return height; }
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; };
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getSensitivity();

	void setSensitivity(float newSensitivity);

	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	~GL_Window();

private:

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	// GLfloat sensitivity;
	// maybe add up down sensitivity

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat sensitivity;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};

