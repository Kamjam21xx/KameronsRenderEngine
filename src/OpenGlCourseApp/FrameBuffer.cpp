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
}
void FrameBuffer::Init(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
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

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it

	// GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
	// glDrawBuffers(1, buffers);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

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
