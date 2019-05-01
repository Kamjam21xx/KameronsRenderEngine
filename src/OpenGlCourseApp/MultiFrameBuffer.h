#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>

#include "FrameBuffer.h"

class MultiFrameBuffer
{
public:

	MultiFrameBuffer();
	MultiFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLint width, GLint height);
	MultiFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	// void Init(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum textureUnitThree, GLenum textureUnitFour,
	//		  GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void Init(GLuint textureUnitOne, GLenum textureUnitTwo,
		GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void BindTexture(unsigned short int index);
	void BindTexture(GLenum textureUnit, unsigned short int index);
	void BindAndSetTexture(GLenum textureUnit, unsigned short int index);
	void BindTextures(GLenum textureUnitOne, GLenum textureUnitTwo);
	void BindAndSetTextures(GLenum textureUnitOne, GLenum textureUnitTwo);

	void SetTextureUnit(GLenum textureUnit, unsigned short int index);

	GLuint GetBufferTextureUnit(unsigned short int index) const;
	GLuint GetTexColorBuffer(unsigned short int index) const;
	GLuint GetFBO() const;
	GLuint GetRBO() const;

	~MultiFrameBuffer();

private:

	// GLuint FBO[4];
	// GLuint RBO[4];
	GLuint FBO;
	GLuint RBO;
	GLuint colorBuffers[4];
	GLuint textureUnits[4];

	GLint bufferWidth, bufferHeight;

	MultiFrameBuffer(MultiFrameBuffer *framebuffer) 
	{
		// FrameBuffer copy constructor may not be called
	}

};



// intentionally uncommented to cause compile failu
