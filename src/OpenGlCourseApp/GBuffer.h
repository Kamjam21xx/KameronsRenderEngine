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
	void BindAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU);
	void BindAndSetAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU);
	void SetAll(unsigned short int PositionTU, unsigned short int NormalHeightTU, unsigned short int ColorSpecularTU, unsigned short int DepthStencilTU);

	void BindTexturePos();
	void BindTexturePos(const unsigned short int textureUnit);
	void BindAndSetTexturePos(const unsigned short int textureUnit);
	void BindTextureNormHeight();
	void BindTextureNormHeight(const unsigned short int textureUnit);
	void BindAndSetTextureNormHeight(const unsigned short int textureUnit);
	void BindTextureColSpec();
	void BindTextureColSpec(const unsigned short int textureUnit);
	void BindAndSetTextureColSpec(const unsigned short int textureUnit);
	void BindTextureDepthStencil();
	void BindTextureDepthStencil(const unsigned short int textureUnit);
	void BindAndSetTextureDepthStencil(const unsigned short int textureUnit);

	void SetTextureUnitPos(const unsigned short int textureUnit);
	void SetTextureUnitNormHeight(const unsigned short int textureUnit);
	void SetTextureUnitColSpec(const unsigned short int textureUnit);
	void SetTextureUnitDepthStencil(const unsigned short int textureUnit);

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
	void SetTextureParameters() const ;

};

