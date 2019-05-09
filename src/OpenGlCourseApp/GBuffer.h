#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

class GBuffer
{
public:
	GBuffer();

	void Init(GLenum PositionTU, GLenum NormalHeightTU, GLenum ColorSpecularTU, GLenum depthStencilTU, GLint width, GLint height);

	void BindAll();
	void BindAll(GLenum PositionTU, GLenum NormalHeightTU, GLenum ColorSpecularTU, GLenum DepthStencilTU);
	void BindAndSetAll(GLenum PositionTU, GLenum NormalHeightTU, GLenum ColorSpecularTU, GLenum DepthStencilTU);
	void SetAll(GLenum PositionTU, GLenum NormalHeightTU, GLenum ColorSpecularTU, GLenum DepthStencilTU);

	void BindTexturePos();
	void BindTexturePos(GLenum textureUnit);
	void BindAndSetTexturePos(GLenum textureUnit);
	void BindTextureNormHeight();
	void BindTextureNormHeight(GLenum textureUnit);
	void BindAndSetTextureNormHeight(GLenum textureUnit);
	void BindTextureColSpec();
	void BindTextureColSpec(GLenum textureUnit);
	void BindAndSetTextureColSpec(GLenum textureUnit);
	void BindTextureDepthStencil();
	void BindTextureDepthStencil(GLenum textureUnit);
	void BindAndSetTextureDepthStencil(GLenum textureUnit);

	void SetTextureUnitPos(GLenum textureUnit);
	void SetTextureUnitNormHeight(GLenum textureUnit);
	void SetTextureUnitColSpec(GLenum textureUnit);
	void SetTextureUnitDepthStencil(GLenum textureUnit);

	GLuint GetTextureUnitPos() const ;
	GLuint GetTextureUnitNormHeight() const ;
	GLuint GetTextureUnitColSpec() const ;
	GLuint GetTextureUnitDepthStencil() const;

	GLuint GetPosBuffer() const ;
	GLuint GetNormHeightBuffer() const ;
	GLuint GetColSpecBuffer() const ;
	GLuint GetDepthStencilBuffer() const;

	GLuint GetFBO() const ;
	GLuint GetRBO() const ;

	~GBuffer();

private:

	GLuint FBO, RBO;
	GLuint Position, NormalHeight, ColorSpecular, DepthStencil;
	GLuint TextureUnitPosition, TextureUnitNormalHeight, TextureUnitColorSpecular, TextureUnitDepthStencil;
	GLint bufferWidth, bufferHeight;

	void AttachDepthStencilRBO();
	void AttachDepthStencilTex();
	void SetTextureParameters();

};

