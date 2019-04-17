#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture(GLenum glTextureUnit);
	bool LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type);
	bool LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering);
	bool LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filteringMIN, GLenum filteringMAX);

	bool GenerateTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLint texWidth, GLint texHeight);
	bool GenerateTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint texWidth, GLint texHeight);

	void UseTexture();
	void ClearTexture();

	

	void SetTextureUnit(GLuint glTextureUnit);
	int GetWidth() const;
	int GetHeight() const;
	int GetBitDepth() const;
	GLuint GetTextureID() const;
	GLuint GetTextureUnit() const;
	const char* GetFileLocationPtr() const;

	~Texture();

private:

	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
	GLenum textureUnit;
};
