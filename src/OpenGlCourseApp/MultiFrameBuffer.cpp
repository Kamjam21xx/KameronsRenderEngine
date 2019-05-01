#include "MultiFrameBuffer.h"



MultiFrameBuffer::MultiFrameBuffer()
{

}
MultiFrameBuffer::MultiFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLint width, GLint height)
{
	Init(textureUnitOne, textureUnitTwo, GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, width, height);
}
MultiFrameBuffer::MultiFrameBuffer(GLuint textureUnitOne, GLenum textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	Init(textureUnitOne, textureUnitTwo, internalFormat, format, type, filtering, width, height);
}


void MultiFrameBuffer::Init(GLuint textureUnitOne, GLenum textureUnitTwo,
							GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	if (FBO || RBO || colorBuffers[0] || colorBuffers[1])
	{
		printf("Error : DualFrameBuffer cannot be re-initialized");
	}
	


	// set texture unit for auto binding from main
	textureUnits[0] = textureUnitOne;
	textureUnits[1] = textureUnitTwo;

	// create dual color attachment textures
	glGenFramebuffers(2, &FBO);
	glGenTextures(2, colorBuffers);
	for (unsigned short int i = 0; i < 2; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// bind framebuffer, then bind texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);

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


// leaving out checks for the sake of speed
void MultiFrameBuffer::BindTexture(unsigned short int index)
{
	glActiveTexture(textureUnits[index]);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
}
void MultiFrameBuffer::BindAndSetTexture(GLenum textureUnit, unsigned short int index)
{
	textureUnits[index] = textureUnit;
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
}
void MultiFrameBuffer::SetTextureUnit(GLenum textureUnit, unsigned short int index)
{
	textureUnits[index] = textureUnit;
}


GLuint MultiFrameBuffer::GetBufferTextureUnit(unsigned short int index) const
{
	return textureUnits[index];
}
GLuint MultiFrameBuffer::GetTexColorBuffer(unsigned short int index) const
{
	return colorBuffers[index];
}
GLuint MultiFrameBuffer::GetFBO() const
{
	return FBO;
}
GLuint MultiFrameBuffer::GetRBO() const
{
	return RBO;
}


MultiFrameBuffer::~MultiFrameBuffer()
{
	if (RBO)
	{
		glDeleteRenderbuffers(1, &RBO);
	}
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (colorBuffers[0])
	{
		glDeleteTextures(1, &colorBuffers[0]);
	}
	if (colorBuffers[1])
	{
		glDeleteTextures(1, &colorBuffers[1]);
	}
	textureUnits[0] = NULL;
	textureUnits[1] = NULL;
}
