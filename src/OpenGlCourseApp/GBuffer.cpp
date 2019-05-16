#include "GBuffer.h"

// be careful to use the correct features of the class depending on if you use an RBO or not, etc.

GBuffer::GBuffer()
{

}

void GBuffer::Init(GLenum PositionTU, GLenum NormalHeightTU, GLenum ColorSpecularTU, GLenum depthStencilTU, GLint width, GLint height)
{
	bufferWidth = width;
	bufferHeight = height;

	TextureUnitPosition = PositionTU;
	TextureUnitNormalHeight = NormalHeightTU;
	TextureUnitColorSpecular = ColorSpecularTU;
	TextureUnitDepthStencil = depthStencilTU;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &Position);
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Position, 0);

	glGenTextures(1, &NormalHeight);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalHeight, 0);

	glGenTextures(1, &ColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, ColorSpecular, 0);

	if (TextureUnitDepthStencil == NULL)
	{
		AttachDepthStencilRBO();
	}
	else
	{
		AttachDepthStencilTex();
	}

	GLenum attachments[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer object : framebuffer incomplete! /n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::BindAll() 
{
	glActiveTexture(GL_TEXTURE0 + TextureUnitPosition);
	glBindTexture(GL_TEXTURE_2D, Position);

	glActiveTexture(GL_TEXTURE0 + TextureUnitNormalHeight);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);

	glActiveTexture(GL_TEXTURE0 + TextureUnitColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);

	glActiveTexture(GL_TEXTURE0 + TextureUnitDepthStencil);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}
void GBuffer::BindAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU)
{
	glActiveTexture(GL_TEXTURE0 + PositionTU);
	glBindTexture(GL_TEXTURE_2D, Position);

	glActiveTexture(GL_TEXTURE0 + NormalHeightTU);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);

	glActiveTexture(GL_TEXTURE0 + ColorSpecularTU);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);

	glActiveTexture(GL_TEXTURE0 + DepthStencilTU);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}
void GBuffer::BindAndSetAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU)
{
	TextureUnitPosition = PositionTU;
	TextureUnitNormalHeight = NormalHeightTU;
	TextureUnitColorSpecular = ColorSpecularTU;
	TextureUnitDepthStencil = DepthStencilTU;

	glActiveTexture(GL_TEXTURE0 + PositionTU);
	glBindTexture(GL_TEXTURE_2D, Position);

	glActiveTexture(GL_TEXTURE0 + NormalHeightTU);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);

	glActiveTexture(GL_TEXTURE0 + ColorSpecularTU);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);

	glActiveTexture(GL_TEXTURE0 + DepthStencilTU);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}
void GBuffer::SetAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU)
{
	TextureUnitPosition = PositionTU;
	TextureUnitNormalHeight = NormalHeightTU;
	TextureUnitColorSpecular = ColorSpecularTU;
	TextureUnitDepthStencil = DepthStencilTU;
}
void GBuffer::BindTexturePos()
{
	glActiveTexture(GL_TEXTURE0 + TextureUnitPosition);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindTexturePos(const unsigned short int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindAndSetTexturePos(const unsigned short int textureUnit)
{
	TextureUnitPosition = textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindTextureNormHeight()
{
	glActiveTexture(GL_TEXTURE0 + TextureUnitNormalHeight);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);
}
void GBuffer::BindTextureNormHeight(const unsigned short int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);
}
void GBuffer::BindAndSetTextureNormHeight(const unsigned short int textureUnit)
{
	TextureUnitNormalHeight = textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, NormalHeight);
}
void GBuffer::BindTextureColSpec()
{
	glActiveTexture(GL_TEXTURE0 + TextureUnitColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindTextureColSpec(const unsigned short int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindAndSetTextureColSpec(const unsigned short int textureUnit)
{
	TextureUnitColorSpecular = textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindTextureDepthStencil()
{
	glActiveTexture(GL_TEXTURE0 + TextureUnitDepthStencil);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}
void GBuffer::BindTextureDepthStencil(const unsigned short int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}
void GBuffer::BindAndSetTextureDepthStencil(const unsigned short int textureUnit)
{
	TextureUnitDepthStencil = textureUnit;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
}

void GBuffer::SetTextureUnitPos(const unsigned short int textureUnit)
{
	TextureUnitPosition = textureUnit;
}
void GBuffer::SetTextureUnitNormHeight(const unsigned short int textureUnit)
{
	TextureUnitNormalHeight = textureUnit;
}
void GBuffer::SetTextureUnitColSpec(const unsigned short int textureUnit)
{
	TextureUnitColorSpecular = textureUnit;
}
void GBuffer::SetTextureUnitDepthStencil(const unsigned short int textureUnit)
{
	TextureUnitDepthStencil = textureUnit;
}

GLuint GBuffer::GetTextureUnitPos() const
{
	return TextureUnitPosition;
}
GLuint GBuffer::GetTextureUnitNormHeight() const
{
	return TextureUnitNormalHeight;
}
GLuint GBuffer::GetTextureUnitColSpec() const
{
	return TextureUnitColorSpecular;
}
GLuint GBuffer::GetTextureUnitDepthStencil() const
{
	return TextureUnitDepthStencil;
}

GLuint GBuffer::GetPosBuffer() const
{
	return Position;
}
GLuint GBuffer::GetNormHeightBuffer() const
{
	return NormalHeight;
}
GLuint GBuffer::GetColSpecBuffer() const
{
	return ColorSpecular;
}
GLuint GBuffer::GetDepthStencilBuffer() const
{
	return DepthStencil;
}

GLuint GBuffer::GetFBO() const
{
	return FBO;
}
GLuint GBuffer::GetRBO() const
{
	return RBO;
}

void GBuffer::AttachDepthStencilRBO()
{
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
void GBuffer::AttachDepthStencilTex()
{
	glGenTextures(1, &DepthStencil);
	glBindTexture(GL_TEXTURE_2D, DepthStencil);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencil, 0);
}

void GBuffer::SetTextureParameters() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GBuffer::~GBuffer()
{
	if (Position)
	{
		glDeleteTextures(1, &Position);
	}
	if (NormalHeight)
	{
		glDeleteTextures(1, &NormalHeight);
	}
	if (ColorSpecular)
	{
		glDeleteTextures(1, &ColorSpecular);
	}
	if (DepthStencil)
	{
		glDeleteTextures(1, &DepthStencil);
	}
	if (RBO)
	{
		glDeleteRenderbuffers(1, &RBO);
	}
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	TextureUnitPosition = NULL;
	TextureUnitNormalHeight = NULL;
	TextureUnitColorSpecular = NULL;
	bufferWidth = NULL;
	bufferHeight = NULL;
}