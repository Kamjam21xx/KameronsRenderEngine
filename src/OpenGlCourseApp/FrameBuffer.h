#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

class FrameBuffer
{
public:

	FrameBuffer();
	FrameBuffer(GLenum textureUnit, GLint width, GLint height);
	FrameBuffer(GLenum textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

    void Init(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void BindTexture();
	void BindTexture(unsigned short int textureUnit);
	void BindAndSetTexture(unsigned short int textureUnit);

	void SetTextureUnit(unsigned short int textureUnit);

	GLuint GetBufferTextureUnit() const;
	GLuint GetTexColorBuffer() const;
	GLuint GetFBO() const;
	GLuint GetRBO() const;

	~FrameBuffer();

private:

	GLuint FBO, RBO, texColorBuffer, bufferTextureUnit;
	GLint bufferWidth, bufferHeight;

	FrameBuffer(FrameBuffer *framebuffer) 
	{
		// FrameBuffer copy constructor may not be called
	}

};

