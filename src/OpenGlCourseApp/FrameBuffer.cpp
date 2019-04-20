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
/*

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

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

*/

	bufferTextureUnit = textureUnit;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
	// create a color attachment texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
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
	glDeleteRenderbuffers(1, &RBO);
	glDeleteFramebuffers(1, &FBO);
}
