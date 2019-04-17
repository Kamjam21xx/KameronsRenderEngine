#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(GLuint textureUnit, GLint width, GLint height)
{

	
	texture.GenerateTexture(textureUnit, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, width, height);


}
FrameBuffer::FrameBuffer(GLuint textureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint width, GLint height)
{
	

	texture.GenerateTexture(textureUnit, internalFormat, format, type, filtering, width, height);


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
