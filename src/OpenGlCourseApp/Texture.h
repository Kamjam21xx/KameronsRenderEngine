#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
	bool LoadTextureData(glm::vec3 *imageData, unsigned short int width, unsigned short int height, GLenum internalFormat, GLenum format, GLenum type);

	bool MergeLoadTexture(GLenum glTextureUnit, const char *fileLocationTwo);

	bool GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLint texWidth, GLint texHeight);
	bool GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint texWidth, GLint texHeight);

	void Unbind() const;

	void UseTexture() const;
	void UseTexture(GLuint textureUnit);
	void ClearTexture();

	void SetTextureUnit(GLuint glTextureUnit);
	int GetWidth() const;
	int GetHeight() const;
	int GetBitDepth() const;
	GLuint GetTextureID() const;
	GLuint GetTextureUnit() const;
	const char* GetFilePath() const;

	~Texture();

private:

	bool LoadTextureData(const char *imageData);

	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
	GLenum textureUnit;
};
