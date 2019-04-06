#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();
	bool LoadTextureRGBA();

	bool LoadTexture(GLenum glTextureUnit);
	bool LoadTextureA(GLenum glTextureUnit);
	bool LoadTextureRGBA(GLenum glTextureUnit);
	bool LoadTextureNormalRGBA(GLenum glTextureUnit);
	bool LoadTextureGreyscale(GLenum glTextureUnit);

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
	GLenum textureUnit;
};

