#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "Texture.h"

class FrameBuffer
{
public:

	GLuint FBO, RBO, texColorBuffer, bufferTextureUnit;
	Texture colorTexture;


	FrameBuffer();
	//FrameBuffer(GLenum textureUnit, GLint width, GLint height);
	FrameBuffer(GLenum textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void BindTexture();
	void BindTexture(GLenum textureUnit);
	void BindAndSetTexture(GLenum textureUnit);

	void SetTextureUnit(GLenum textureUnit);
	GLuint GetFBO() const;
	GLuint GetRBO() const;

	~FrameBuffer();

private:

	GLint bufferWidth, bufferHeight;

};

