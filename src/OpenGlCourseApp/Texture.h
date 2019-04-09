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

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
	GLenum textureUnit;
};
