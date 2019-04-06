#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"



class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName, GLenum drawType, bool tangents);
	void LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, bool tangents);
	void LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, bool tangents);
	void LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, bool tangents);
	void LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture, bool tangents);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	void LoadNode(aiNode *node, const aiScene *scene, GLenum drawType, bool tangents);
	void LoadMesh(aiMesh *mesh, const aiScene *scene, GLenum drawType, bool tangents);
	// void LoadMesh(aiMesh *mesh, const aiScene *scene, GLenum drawType, std::string colorTexture);      oops idr
	void LoadMaterials(const aiScene *scene);
	void LoadMaterials(const aiScene *scene, std::string colorTexture);
	void LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture);
	void LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture);
	void LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureListDiffuse;
	std::vector<Texture*> textureListSpecular;
	std::vector<Texture*> textureListNormal;
	std::vector<Texture*> textureListHeight;
	std::vector<unsigned int> meshToTex;
};

