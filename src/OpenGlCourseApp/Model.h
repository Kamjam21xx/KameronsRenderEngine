#pragma once

#include <vector>
#include <string>
#include <bitset>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"



class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture, std::bitset<8> bitSet);
	
	void RenderModel();
	void ClearModel();

	~Model();

private:
	void LoadNode(aiNode *node, const aiScene *scene, GLenum drawType, bool tangents);
	void LoadMesh(aiMesh *mesh, const aiScene *scene, GLenum drawType, bool tangents);

	void LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture);

	std::vector<Mesh*> meshList;
	
	std::vector<Texture*> textureListDiffuse;
	std::vector<Texture*> textureListSpecular;
	std::vector<Texture*> textureListNormal;
	std::vector<Texture*> textureListHeight;
	
	std::vector<unsigned int> meshToTex;

	std::vector<std::bitset<8>> bitFlags;
};

