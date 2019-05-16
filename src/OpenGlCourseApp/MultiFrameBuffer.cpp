#include "MultiFrameBuffer.h"



MultiFrameBuffer::MultiFrameBuffer()
{
	isInstantiated = false;
}
MultiFrameBuffer::MultiFrameBuffer(unsigned short int textureUnitOne, unsigned short int textureUnitTwo, GLshort width, GLshort height)
{
	Init(textureUnitOne, textureUnitTwo, GL_RGB, GL_RGB, GL_FLOAT, GL_LINEAR, width, height);
	isInstantiated = true;
}
MultiFrameBuffer::MultiFrameBuffer(unsigned short int textureUnitOne, unsigned short int textureUnitTwo, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLshort width, GLshort height)
{
	Init(textureUnitOne, textureUnitTwo, internalFormat, format, type, filtering, width, height);
	isInstantiated = true;
}


void MultiFrameBuffer::Init(unsigned short int textureUnitOne, unsigned short int textureUnitTwo,
	GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLshort width, GLshort height)
{
	if (isInstantiated)
	{
		printf("Error : DualFrameBuffer cannot be re-initialized");
	}
	else
	{
		// set texture unit for auto binding from main
		textureUnits[0] = textureUnitOne;
		textureUnits[1] = textureUnitTwo;

		// create dual color attachment textures
		glGenFramebuffers(1, FBO);
		glGenTextures(2, colorBuffers);
		for (unsigned short int i = 0; i < 2; ++i)
		{
			InitTexture(i, internalFormat, width, height, format, type, filtering);
			AttachTextureToFBO(0, i);
			AttachDepthStencilRBO(i, width, height);
		}

		// set draw buffers and color attachments
		GLenum buffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, buffers);
		glReadBuffer(GL_NONE);

		// check framebuffer for completeness 
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FrameBuffer object : framebuffer incomplete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void MultiFrameBuffer::InitPingPong(unsigned short int textureUnit,
	GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLshort width, GLshort height)
{
	if (isInstantiated)
	{
		printf("Error : DualFrameBuffer cannot be re-initialized");
	}
	else
	{ 
		// set texture unit for auto binding from main
		textureUnits[0] = textureUnit;
		textureUnits[1] = textureUnit;

		// create dual color attachment textures
		glGenFramebuffers(2, FBO);
		glGenTextures(2, colorBuffers);
		for (unsigned short int i = 0; i < 2; ++i)
		{
			InitTexture(i, internalFormat, width, height, format, type, filtering);
			AttachTextureToFBO(i, i, 0);
		}

		// set draw buffer/read and color attachment
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		// check framebuffer for completeness 
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FrameBuffer object : framebuffer incomplete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void MultiFrameBuffer::InitTexture(unsigned short int index, GLenum internalFormat, GLint width, GLint height, GLenum format, GLenum type, GLenum filtering)
{		
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void MultiFrameBuffer::AttachTextureToFBO(unsigned short int indexFBO, unsigned short int indexColor, unsigned short int indexColorAttachment)
{
	// bind framebuffer, then bind texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[indexFBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexColorAttachment, GL_TEXTURE_2D, colorBuffers[indexColor], 0);
}
void MultiFrameBuffer::AttachTextureToFBO(unsigned short int indexFBO, unsigned short int indexColor)
{
	// bind framebuffer, then bind texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO[indexFBO]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexColor, GL_TEXTURE_2D, colorBuffers[indexColor], 0);
}
void MultiFrameBuffer::AttachDepthStencilRBO(unsigned short int indexRBO, GLint width, GLint height)
{
	// create a renderbuffer object for depth and stencil
	glGenRenderbuffers(1, &RBO[indexRBO]);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO[indexRBO]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO[indexRBO]);
}

// leaving out checks for the sake of speed, user beware.
void MultiFrameBuffer::BindTexture(unsigned short int index)
{
	glActiveTexture(GL_TEXTURE0 + textureUnits[index]);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
}
void MultiFrameBuffer::BindTexture(unsigned short int textureUnit, unsigned short int index)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
}
void MultiFrameBuffer::BindAndSetTexture(unsigned short int textureUnit, unsigned short int index)
{
	textureUnits[index] = textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[index]);
}
void MultiFrameBuffer::BindTextures(unsigned short int textureUnitOne, unsigned short int textureUnitTwo)
{
	glActiveTexture(GL_TEXTURE0 + textureUnitOne);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

	glActiveTexture(GL_TEXTURE0 + textureUnitTwo);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
}
void MultiFrameBuffer::BindAndSetTextures(unsigned short int textureUnitOne, unsigned short int textureUnitTwo)
{
	textureUnits[0] = textureUnitOne;
	textureUnits[1] = textureUnitTwo;

	glActiveTexture(GL_TEXTURE0 + textureUnitOne);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);

	glActiveTexture(GL_TEXTURE0 + textureUnitTwo);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
}


void MultiFrameBuffer::SetTextureUnit(unsigned short int textureUnit, unsigned short int index)
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
GLuint MultiFrameBuffer::GetFBO(unsigned short int index) const
{
	return FBO[index];
}
GLuint MultiFrameBuffer::GetRBO(unsigned short int index) const
{
	return RBO[index];
}


MultiFrameBuffer::~MultiFrameBuffer()
{
	for (unsigned short int i = 0; i < 4; i++)
	{
		if (RBO[i])
		{
			glDeleteRenderbuffers(1, &RBO[i]);
		}
		if (FBO[i])
		{
			glDeleteFramebuffers(1, &FBO[i]);
		}
		if (colorBuffers[i])
		{
			glDeleteTextures(1, &colorBuffers[i]);
		}
		if (colorBuffers[i])
		{
			glDeleteTextures(1, &colorBuffers[i]);
		}

		textureUnits[i] = NULL;
	}



}
