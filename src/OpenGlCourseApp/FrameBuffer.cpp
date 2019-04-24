#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{

}
FrameBuffer::FrameBuffer(GLuint textureUnit, GLint width, GLint height)
{
	Init(textureUnit, GL_SRGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR, width, height);
}
FrameBuffer::FrameBuffer(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	Init(textureUnit, internalFormat, format, type, filtering, width, height);
}

void FrameBuffer::Init(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	if (FBO || RBO || texColorBuffer) 
	{
		printf("Error : Framebuffer cannot be re-initialized");
		return;
	}

	// set texture unit for auto binding from main
	bufferTextureUnit = textureUnit;

	// generate framebuffer
	glGenFramebuffers(1, &FBO);

	// create a color attachment texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	// bind framebuffer, then bind texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// set color attachment 
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer object : framebuffer incomplete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexture()
{
	glActiveTexture(bufferTextureUnit);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}
void FrameBuffer::BindTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}
void FrameBuffer::BindAndSetTexture(GLenum textureUnit)
{
	bufferTextureUnit = textureUnit;

	glActiveTexture(bufferTextureUnit);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
}

void FrameBuffer::SetTextureUnit(GLenum textureUnit)
{
	bufferTextureUnit = textureUnit;
}

GLuint FrameBuffer::GetBufferTextureUnit() const
{
	return bufferTextureUnit;
}
GLuint FrameBuffer::GetTexColorBuffer() const
{
	return texColorBuffer;
}
GLuint FrameBuffer::GetFBO() const
{
	return FBO;
}
GLuint FrameBuffer::GetRBO() const
{
	return RBO;
}


FrameBuffer::~FrameBuffer()
{	
	if (RBO) 
	{
		glDeleteRenderbuffers(1, &RBO);
	}
	if (FBO) 
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (texColorBuffer) 
	{
		glDeleteTextures(1, &texColorBuffer);
	}
}
