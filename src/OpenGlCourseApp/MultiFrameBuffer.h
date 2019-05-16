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
	MultiFrameBuffer(unsigned short int textureUnitOne, unsigned short int textureUnitTwo, GLshort width, GLshort height);
	MultiFrameBuffer(unsigned short int textureUnitOne, unsigned short int textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLshort width, GLshort height);

	// void Init(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum textureUnitThree, GLenum textureUnitFour,
	//		  GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height);

	void Init(unsigned short int textureUnitOne, unsigned short int textureUnitTwo,
		GLenum internalFormat, GLenum format, GLenum type, GLenum filtering,
		GLshort width, GLshort height);

	void InitPingPong(unsigned short int textureUnit,
					  GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, 
		GLshort width, GLshort height);

	void BindTexture(unsigned short int index);
	void BindTexture(unsigned short int textureUnit, unsigned short int index);
	void BindAndSetTexture(unsigned short int textureUnit, unsigned short int index);
	void BindTextures(unsigned short int textureUnitOne, unsigned short int textureUnitTwo);
	void BindAndSetTextures(unsigned short int textureUnitOne, unsigned short int textureUnitTwo);

	void SetTextureUnit(unsigned short int textureUnit, unsigned short int index);

	GLuint GetBufferTextureUnit(unsigned short int index) const;
	GLuint GetTexColorBuffer(unsigned short int index) const;
	GLuint GetFBO(unsigned short int index) const;
	GLuint GetRBO(unsigned short int index) const;

	~MultiFrameBuffer();
	
private:

	bool isInstantiated;
	
	GLuint FBO[4];
	GLuint RBO[4];

	GLuint colorBuffers[4];
	GLuint textureUnits[4];

	GLshort bufferWidth, bufferHeight;

	MultiFrameBuffer(MultiFrameBuffer *framebuffer) 
	{
		// FrameBuffer copy constructor may not be called
	}

	void InitTexture(unsigned short int index, GLenum internalFormat, GLint width, GLint height, GLenum format, GLenum type, GLenum filtering);
	void AttachTextureToFBO(unsigned short int indexFBO, unsigned short int indexColor, unsigned short int indexColorAttachment);
	void AttachTextureToFBO(unsigned short int indexFBO, unsigned short int indexColor);
	void AttachDepthStencilRBO(unsigned short int indexRBO, GLint width, GLint height);



};



// intentionally uncommented to cause compile failu
