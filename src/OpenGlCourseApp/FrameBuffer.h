#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "Texture.h"

class FrameBuffer
{
public:
	
	FrameBuffer(GLenum textureUnit, GLint width, GLint height);
	FrameBuffer(GLenum textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	GLuint GetFBO();

	void SetTextureUnit(GLuint textureUnit);
	

	~FrameBuffer();

private:
	FrameBuffer();

	Texture texture;
	GLint bufferWidth, bufferHeight;
	GLuint FBO;


};

