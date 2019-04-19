#include "Texture.h"


Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL; 
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

bool Texture::LoadTexture(GLenum glTextureUnit) 
{
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8); // edit func for anisotropy manipulation

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D); 

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
	return true;
}

bool Texture::LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type) 
{
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
	return true;
}

bool Texture::LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering) 
{
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
	return true;
}

bool Texture::LoadTexture(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filteringMIN, GLenum filteringMAX) 
{
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringMIN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringMAX);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
	return true;
}


bool Texture::GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLint texWidth, GLint texHeight)
{
	// used for FrameBuffer class texture
	textureUnit = glTextureUnit;
	height = texHeight;
	width = texWidth;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texHeight, texHeight, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

bool Texture::GenerateTextureFBO(GLenum glTextureUnit, GLenum internalFormat, GLenum format, GLenum type, GLenum filtering, GLint texWidth, GLint texHeight)
{
	// used for FrameBuffer class texture
	textureUnit = glTextureUnit;
	height = texHeight;
	width = texWidth;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texWidth, texHeight, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	return true;
}

void Texture::BindZero() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::ClearTexture() 
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}
void Texture::UseTexture() const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID); 
}


void Texture::SetTextureUnit(GLuint glTextureUnit) 
{
	textureUnit = glTextureUnit;
}
int Texture::GetWidth() const
{
	return width;
}
int Texture::GetHeight() const
{
	return height;
}
int Texture::GetBitDepth() const
{
	return bitDepth;
}
GLuint Texture::GetTextureID() const
{
	return textureID;
}
GLuint Texture::GetTextureUnit() const
{
	return textureUnit;
}
const char* Texture::GetFileLocationPtr() const 
{
	return fileLocation;
}


Texture::~Texture()
{
	ClearTexture();
}
