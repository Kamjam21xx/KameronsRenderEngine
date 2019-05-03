#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

class GBuffer
{
public:
	GBuffer();

	void Init(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU, GLint width, GLint height);
	void Init(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU, GLint width, GLint height, bool useRenderBuffer);

	void BindAll();
	void BindAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU);
	void BindAndSetAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU);
	void SetAll(GLenum PositionTU, GLenum NormalTU, GLenum ColorSpecularTU);

	void BindTexturePos();
	void BindTexturePos(GLenum textureUnit);
	void BindAndSetTexturePos(GLenum textureUnit);
	void BindTextureNorm();
	void BindTextureNorm(GLenum textureUnit);
	void BindAndSetTextureNorm(GLenum textureUnit);
	void BindTextureColSpec();
	void BindTextureColSpec(GLenum textureUnit);
	void BindAndSetTextureColSpec(GLenum textureUnit);

	void SetTextureUnitPos(GLenum textureUnit);
	void SetTextureUnitNorm(GLenum textureUnit);
	void SetTextureUnitColSpec(GLenum textureUnit);

	GLuint GetTextureUnitPos() const ;
	GLuint GetTextureUnitNorm() const ;
	GLuint GetTextureUnitColSpec() const ;

	GLuint GetPosBuffer() const ;
	GLuint GetNormBuffer() const ;
	GLuint GetColSpec() const ;

	GLuint GetFBO() const ;
	GLuint GetRBO() const ;

	~GBuffer();

private:

	GLuint FBO, RBO;
	GLuint Position, Normal, ColorSpecular;
	GLuint TextureUnitPosition, TextureUnitNormal, TextureUnitColorSpecular;
	GLint bufferWidth, bufferHeight;
	bool useRBO;

	void AttachRBO(GLint width, GLint height);
	void SetTextureParameters();

};

