#include "DualFrameBuffer.h"



DualFrameBuffer::DualFrameBuffer()
{

}
DualFrameBuffer::DualFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLint width, GLint height)
{
	Init(textureUnitOne, textureUnitTwo, GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, width, height);
}
DualFrameBuffer::DualFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	Init(textureUnitOne, textureUnitTwo, internalFormat, format, type, filtering, width, height);
}

void DualFrameBuffer::Init(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	if (FBO || RBO || texColorBuffers[0] || texColorBuffers[1])
	{
		printf("Error : DualFrameBuffer cannot be re-initialized");
	}

	// set texture unit for auto binding from main
	bufferTextureUnits[0] = textureUnitOne;
	bufferTextureUnits[1] = textureUnitTwo;

	// create dual color attachment textures
	glGenFramebuffers(2, &FBO);
	glGenTextures(2, texColorBuffers);
	for (unsigned short int i = 0; i < 2; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, texColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // ??? shouldnt matter, but adding to be safe
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // ??? shouldnt matter, but adding to be safe

		// bind framebuffer, then bind texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texColorBuffers[i], 0);

		// create a renderbuffer object for depth and stencil
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}
	
	// set draw buffers and color attachments
	GLenum buffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer object : framebuffer incomplete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DualFrameBuffer::BindTexture(unsigned short int index)
{
	glActiveTexture(bufferTextureUnits[index]);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[index]);
}
void DualFrameBuffer::BindTexture(GLenum textureUnit, unsigned short int index)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[index]);
}
void DualFrameBuffer::BindAndSetTexture(GLenum textureUnit, unsigned short int index)
{
	bufferTextureUnits[index] = textureUnit;
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[index]);
}
void DualFrameBuffer::BindTextures()
{
	glActiveTexture(bufferTextureUnits[0]);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[0]);

	glActiveTexture(bufferTextureUnits[1]);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[1]);
}
void DualFrameBuffer::BindTextures(GLenum textureUnitOne, GLenum textureUnitTwo)
{
	glActiveTexture(textureUnitOne);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[0]);

	glActiveTexture(textureUnitTwo);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[1]);
}
void DualFrameBuffer::BindAndSetTextures(GLenum textureUnitOne, GLenum textureUnitTwo)
{
	bufferTextureUnits[0] = textureUnitOne;
	bufferTextureUnits[1] = textureUnitTwo;

	glActiveTexture(textureUnitOne);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[0]);

	glActiveTexture(textureUnitTwo);
	glBindTexture(GL_TEXTURE_2D, texColorBuffers[1]);
}

void DualFrameBuffer::SetTextureUnit(GLenum textureUnit, unsigned short int index)
{
	bufferTextureUnits[index] = textureUnit;
}

GLuint DualFrameBuffer::GetBufferTextureUnit(unsigned short int index) const
{
	return bufferTextureUnits[index];
}
GLuint DualFrameBuffer::GetTexColorBuffer(unsigned short int index) const
{
	return texColorBuffers[index];
}
GLuint DualFrameBuffer::GetFBO() const
{
	return FBO;
}
GLuint DualFrameBuffer::GetRBO() const
{
	return RBO;
}

DualFrameBuffer::~DualFrameBuffer()
{
	if (RBO)
	{
		glDeleteRenderbuffers(1, &RBO);
	}
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (texColorBuffers[0])
	{
		glDeleteTextures(1, &texColorBuffers[0]);
	}
	if (texColorBuffers[1])
	{
		glDeleteTextures(1, &texColorBuffers[1]);
	}
	bufferTextureUnits[0] = NULL;
	bufferTextureUnits[1] = NULL;
}
