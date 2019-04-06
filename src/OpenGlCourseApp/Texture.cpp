#include "Texture.h"


Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL; // check c++17 reference
	textureUnit = GL_TEXTURE1;
}
Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
	textureUnit = GL_TEXTURE1;
}

bool Texture::LoadTextureA() {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTexture() {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTextureA(GLenum glTextureUnit) {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	textureUnit = glTextureUnit;
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTexture(GLenum glTextureUnit) {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	textureUnit = glTextureUnit;
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTextureRGBA() {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTextureRGBA(GLenum glTextureUnit) {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	textureUnit = glTextureUnit;
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTextureNormalRGBA(GLenum glTextureUnit) {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	textureUnit = glTextureUnit;
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set to RGBA if you want an alpha channel, set up new constructors for all types
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}
bool Texture::LoadTextureGreyscale(GLenum glTextureUnit) {
	// loads image. one and done.
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	textureUnit = glTextureUnit;
	if (!texData) {
		printf("Failed to find: %s \n");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);
	// see notes on paper
	glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps automatically
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData); // frees up raw data at ->texData
	return true;
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}
void Texture::UseTexture() {

	// textureUnit >= GL_TEXTURE1 
	glActiveTexture(textureUnit); // should be at least 16 texture units for binding multiple textures
	glBindTexture(GL_TEXTURE_2D, textureID); 
	// you can combine matrices to fade between colors and other variables
	// note to self : build custom functions for using multiple textures
}


Texture::~Texture()
{
	ClearTexture();
}
