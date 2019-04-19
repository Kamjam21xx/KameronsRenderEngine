#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{

}
/*
FrameBuffer::FrameBuffer(GLuint textureUnit, GLint width, GLint height)
{
	glGenFramebuffers(1, &FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		bufferWidth = width;
		bufferHeight = height;
		colorTexture.GenerateTextureFBO(textureUnit, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.GetTextureID(), 0);




		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FBO);
	}
	else
	{
		printf("FrameBuffer object instantiation failed");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);
}
*/
FrameBuffer::FrameBuffer(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		bufferWidth = width;
		bufferHeight = height;

		// set color component
		colorTexture.GenerateTextureFBO(textureUnit, internalFormat, format, type, filtering, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.GetTextureID(), 0);	

		// add depth & stencil component
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // single rbo for both depth and stencil
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// check if FrameBuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FrameBuffer object : framebuffer incomplete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else 
	{
		printf("FrameBuffer object instantiation failed");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FBO);
	}
}

GLuint FrameBuffer::GetFBO() const
{
	return FBO;
}
GLuint FrameBuffer::GetRBO() const
{
	return RBO;
}

void FrameBuffer::SetTextureUnit(GLuint textureUnit) 
{
	colorTexture.SetTextureUnit(textureUnit);
}

FrameBuffer::~FrameBuffer()
{	
	glDeleteRenderbuffers(1, &RBO);
	glDeleteFramebuffers(1, &FBO);
}
