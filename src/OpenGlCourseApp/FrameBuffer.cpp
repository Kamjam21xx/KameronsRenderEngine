#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(GLuint textureUnit, GLint width, GLint height)
{
	glGenFramebuffers(1, &FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		bufferWidth = width;
		bufferHeight = height;
		texture.GenerateTextureFBO(textureUnit, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureID(), 0);




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
FrameBuffer::FrameBuffer(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	glGenFramebuffers(1, &FBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		bufferWidth = width;
		bufferHeight = height;
		texture.GenerateTextureFBO(textureUnit, internalFormat, format, type, filtering, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureID(), 0);	
		
		glTexImage2D(GL_TEXTURE_2D)
		
		
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FBO);
	}
	else 
	{
		printf("FrameBuffer object instantiation failed");
	}

}



GLuint FrameBuffer::GetFBO() 
{
	return FBO;
}

void FrameBuffer::SetTextureUnit(GLuint textureUnit) 
{
	texture.SetTextureUnit(textureUnit);
}

FrameBuffer::~FrameBuffer()
{
}
