#include "GBuffer.h"



GBuffer::GBuffer()
{

}

void GBuffer::Init(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU, GLint width, GLint height)
{
	Init(PositionTU, NormalTU, ColorSpecularTU, width, height, false);
}
void GBuffer::Init(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU, GLint width, GLint height, bool useRenderBuffer)
{
	useRBO = useRenderBuffer;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &Position);
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Position, 0);

	glGenTextures(1, &Normal);
	glBindTexture(GL_TEXTURE_2D, Normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Normal, 0);

	glGenTextures(1, &ColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	SetTextureParameters();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, ColorSpecular, 0);

	if (useRBO)
	{
		AttachRBO(width, height);
	}

	GLenum attachments [3] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glDrawBuffers(3, attachments);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer object : framebuffer incomplete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::BindAll()
{
	glActiveTexture(TextureUnitPosition);
	glBindTexture(GL_TEXTURE_2D, Position);
	glActiveTexture(TextureUnitNormal);
	glBindTexture(GL_TEXTURE_2D, Normal);
	glActiveTexture(TextureUnitColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU)
{
	glActiveTexture(PositionTU);
	glBindTexture(GL_TEXTURE_2D, Position);
	glActiveTexture(NormalTU);
	glBindTexture(GL_TEXTURE_2D, Normal);
	glActiveTexture(ColorSpecularTU);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindAndSetAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU)
{
	TextureUnitPosition = PositionTU;
	TextureUnitNormal = NormalTU;
	TextureUnitColorSpecular = ColorSpecularTU;

	glActiveTexture(PositionTU);
	glBindTexture(GL_TEXTURE_2D, Position);
	glActiveTexture(NormalTU);
	glBindTexture(GL_TEXTURE_2D, Normal);
	glActiveTexture(ColorSpecularTU);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::SetAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU)
{
	TextureUnitPosition = PositionTU;
	TextureUnitNormal = NormalTU;
	TextureUnitColorSpecular = ColorSpecularTU;
}

void GBuffer::BindTexturePos()
{
	glActiveTexture(TextureUnitPosition);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindTexturePos(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindAndSetTexturePos(GLenum textureUnit)
{
	TextureUnitPosition = textureUnit;
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, Position);
}
void GBuffer::BindTextureNorm()
{
	glActiveTexture(TextureUnitNormal);
	glBindTexture(GL_TEXTURE_2D, Normal);
}
void GBuffer::BindTextureNorm(GLenum textureUnit)
{	
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, Normal);
}
void GBuffer::BindAndSetTextureNorm(GLenum textureUnit)
{
	TextureUnitNormal = textureUnit;
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, Normal);
}
void GBuffer::BindTextureColSpec()
{
	glActiveTexture(TextureUnitColorSpecular);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindTextureColSpec(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}
void GBuffer::BindAndSetTextureColSpec(GLenum textureUnit)
{
	TextureUnitColorSpecular = textureUnit;
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ColorSpecular);
}

void GBuffer::SetTextureUnitPos(GLenum textureUnit)
{
	TextureUnitPosition = textureUnit;
}
void GBuffer::SetTextureUnitNorm(GLenum textureUnit)
{
	TextureUnitNormal = textureUnit;
}
void GBuffer::SetTextureUnitColSpec(GLenum textureUnit)
{
	TextureUnitColorSpecular = textureUnit;
}

GLuint GBuffer::GetTextureUnitPos() const
{
	return TextureUnitPosition;
}
GLuint GBuffer::GetTextureUnitNorm() const
{
	return TextureUnitNormal;
}
GLuint GBuffer::GetTextureUnitColSpec() const
{
	return TextureUnitColorSpecular;
}

GLuint GBuffer::GetPosBuffer() const
{
	return Position;
}
GLuint GBuffer::GetNormBuffer() const
{
	return Normal;
}
GLuint GBuffer::GetColSpec() const
{
	return ColorSpecular;
}

GLuint GBuffer::GetFBO() const
{
	return FBO;
}
GLuint GBuffer::GetRBO() const
{
	return RBO;
}

void GBuffer::AttachRBO(GLint width, GLint height)
{
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
void GBuffer::SetTextureParameters()
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
	if (Normal)
	{
		glDeleteTextures(1, &Normal);
	}
	if (ColorSpecular)
	{
		glDeleteTextures(1, &ColorSpecular);
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
	TextureUnitNormal = NULL;
	TextureUnitColorSpecular = NULL;
	bufferWidth = NULL;
	bufferHeight = NULL;
}