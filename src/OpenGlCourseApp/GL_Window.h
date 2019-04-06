#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GL_Window
{
public:
	GL_Window();
	GL_Window(GLint windowWidth, GLint windowHeight);	
	GL_Window(GLint windowWidth, GLint windowHeight, GLfloat sensitivity);
	int Initialize();
	GLfloat getAspectRatio() { return ( (GLfloat)bufferWidth / (GLfloat)bufferHeight ); } // note: check preformance of encapsulation vs inline
	GLint getWindowWidth() { return width; }
	GLint getWindowHeight() { return height; }
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	// change to use getters and setters in the future
	bool* getKeys() { return keys; };
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getSensitivity();

	void setSensitivity(float newSensitivity);

	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	~GL_Window();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // maybe change to 256 - 348 - 512 

	// GLfloat sensitivity;
	// maybe add up down sensitivity

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat sensitivity;
	bool mouseFirstMoved; // avoid stupid bugs by checking if the mouse actually moved

	void createCallbacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};

