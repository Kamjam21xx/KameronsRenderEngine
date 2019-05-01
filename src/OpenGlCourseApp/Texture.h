#pragma once

#include <GL/glew.h>

#include <string>

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


	bool MergeLoadTexture(GLenum glTextureUnit, const char *fileLocationTwo);

	bool GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLint texWidth, GLint texHeight);
	bool GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint texWidth, GLint texHeight);

	void BindZero() const;

	void UseTexture() const;
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

	bool LoadTextureData(const char *imageData);

	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
	GLenum textureUnit;
};
