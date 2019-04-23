#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

class DualFrameBuffer
{
public:

	DualFrameBuffer();
	DualFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLint width, GLint height);
	DualFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void Init(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void BindTexture(unsigned short int index);
	void BindTexture(GLenum textureUnit, unsigned short int index);
	void BindAndSetTexture(GLenum textureUnit, unsigned short int index);
	void BindTextures();
	void BindTextures(GLenum textureUnitOne, GLenum textureUnitTwo);
	void BindAndSetTextures(GLenum textureUnitOne, GLenum textureUnitTwo);

	void SetTextureUnit(GLenum textureUnit, unsigned short int index);

	GLuint GetBufferTextureUnit(unsigned short int index) const;
	GLuint GetTexColorBuffer(unsigned short int index) const;
	GLuint GetFBO() const;
	GLuint GetRBO() const;

	~DualFrameBuffer();

private:

	GLuint FBO, RBO;

	GLuint texColorBuffers[2];
	GLuint bufferTextureUnits[2];

	GLint bufferWidth, bufferHeight;

	DualFrameBuffer(DualFrameBuffer *framebuffer) {
		// FrameBuffer copy constructor may not be called
	}

};