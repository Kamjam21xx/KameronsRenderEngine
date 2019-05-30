#include "Model.h"
#include <iostream>

Model::Model()
{
	// may use comparison against bitset instead of string in loadMaterial() function
	// undecided. under construction.
}

void Model::LoadModel(const std::string &fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture, std::bitset<8> bitSet) {

	LoadModel(fileName, &drawType, &colorTexture, &specularTexture, &normalTexture, &heightTexture, &bitSet);

}
void Model::LoadModel(const std::string &fileName, GLenum drawType, std::string *colorTexture, std::string *specularTexture, std::string *normalTexture, std::string *heightTexture, std::bitset<8> *bitSet) {

	LoadModel(fileName, &drawType, colorTexture, specularTexture, normalTexture, heightTexture, bitSet);

}
void Model::LoadModel(const std::string& fileName, GLenum *drawType, std::string *colorTexture, std::string *specularTexture, std::string *normalTexture, std::string *heightTexture, std::bitset<8> *bitSet) {
	Assimp::Importer importer;

	aiPostProcessSteps calculateTangents;
	if ((*bitSet)[1]) { // complex bitflag shit, flippin ur bit flags. yeehaw
		calculateTangents = aiProcess_CalcTangentSpace;
	}
	else {
		calculateTangents = aiPostProcessSteps(0);
	}
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs
		| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | calculateTangents);


	if (!scene) {
		printf("Model failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	bitFlags.push_back(*bitSet);
	LoadNode(scene->mRootNode, scene, drawType, (*bitSet)[1]);
	LoadMaterials(scene, colorTexture, specularTexture, normalTexture, heightTexture);
}

void Model::RenderModel() const 
{
	for (size_t i = 0; i < meshList.size(); ++i) 
	{
		unsigned int materialIndex = meshToTex[i];
		
		if (materialIndex < textureListChannelOne.size() && textureListChannelOne[materialIndex]) {
			textureListChannelOne[materialIndex]->UseTexture();
		}
		if (materialIndex < textureListChannelTwo.size() && textureListChannelTwo[materialIndex]) {
			textureListChannelTwo[materialIndex]->UseTexture();
		}

		/*
		if (materialIndex < textureListChannelThree.size() && textureListChannelThree[materialIndex]) {
			textureListChannelThree[materialIndex]->UseTexture();
		}
		if (materialIndex < textureListChannelFour.size() && textureListChannelFour[materialIndex]) {
			textureListChannelFour[materialIndex]->UseTexture();
		}
        */

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel() { // will need to be revised with a pointer count pointing to what we are deleting and the pointers

	for (size_t i = 0; i < meshList.size(); ++i) 
	{
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}

		if (textureListChannelOne[i]) {
			delete textureListChannelOne[i];
			textureListChannelOne[i] = nullptr;
		}

		if (textureListChannelTwo[i]) {
			delete textureListChannelTwo[i];
			textureListChannelTwo[i] = nullptr;
		}

		if (textureListChannelThree[i]) {
			delete textureListChannelThree[i];
			textureListChannelThree[i] = nullptr;
		}

		if (textureListChannelFour[i]) {
			delete textureListChannelFour[i];
			textureListChannelFour[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode *node, const aiScene *scene, GLenum *drawType, bool tangents) 
{
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, drawType, tangents);
	}

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		LoadNode(node->mChildren[i], scene, drawType, tangents);
	}
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene, GLenum *drawType, bool tangents) 
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	if(tangents) {
		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			vertices.insert(vertices.end(), { mesh->mVertices[i].x,
											  mesh->mVertices[i].y,
											  mesh->mVertices[i].z });
			if (mesh->mTextureCoords[0]) {
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f });
			}
			vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z, });
		
			vertices.insert(vertices.end(), { mesh->mTangents[i].x,
											mesh->mTangents[i].y,
											mesh->mTangents[i].z });

			vertices.insert(vertices.end(), { mesh->mBitangents[i].x,
												mesh->mBitangents[i].y,
												mesh->mBitangents[i].z }) ;
		}
	}
	else {
		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			vertices.insert(vertices.end(), { mesh->mVertices[i].x,
											  mesh->mVertices[i].y,
											  mesh->mVertices[i].z });
			if (mesh->mTextureCoords[0]) {
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f });
			}
			vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z, });
		}
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh *newMesh = new Mesh();
	if (tangents) {
		newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size(), *drawType);
	}
	else {
		newMesh->CreateMeshNoTangents(&vertices[0], &indices[0], vertices.size(), indices.size(), *drawType);
	}
	
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *scene, std::string *colorTexture, std::string *specularTexture, std::string *normalTexture, std::string *heightTexture) 
{
	// Can add extra texture channels if need be, cleaned up a touch for now.
	textureListChannelOne.resize(scene->mNumMaterials);
	textureListChannelTwo.resize(scene->mNumMaterials);


	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];


		textureListChannelOne[i] = nullptr;
		if (!textureListChannelOne[i]) 
		{
			const char* textureFile = (*colorTexture).c_str();
			const char* textureFileTwo = (*specularTexture).c_str();

			textureListChannelOne[i] = new Texture(textureFile);
			textureListChannelOne[i]->MergeLoadTexture(GL_TEXTURE1, textureFileTwo);
		}

		textureListChannelTwo[i] = nullptr;
		if (!textureListChannelTwo[i]) {
			const char* textureFile = (*normalTexture).c_str();
			const char* textureFileTwo = (*heightTexture).c_str();

			textureListChannelTwo[i] = new Texture(textureFile);
			textureListChannelTwo[i]->MergeLoadTexture(GL_TEXTURE5, textureFileTwo);
		}

	}
}

// add metal

Model::~Model()
{
	// clean up all data via for loops at pointers
}

















/*










void Model::LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture) {
	textureListChannelOne.resize(scene->mNumMaterials);
	textureListChannelThree.resize(scene->mNumMaterials);
	textureListChannelTwo.resize(scene->mNumMaterials);
	textureListChannelFour.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListChannelOne[i] = nullptr;
		if (colorTexture != "") {
			if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					int idx = std::string(path.data).rfind("\\");
					std::string filename = std::string(path.data).substr(idx + 1);

					std::string texPath = std::string("Textures/") + filename;

					textureListChannelOne[i] = new Texture(texPath.c_str());

					if (!textureListChannelOne[i]->LoadTexture(GL_TEXTURE1)) {
						printf("Failed to load texture at: %s \n", texPath);
						delete textureListChannelOne[i];
						textureListChannelOne[i] = nullptr;
					}
				}
			}
			if (!textureListChannelOne[i]) {
				const char* textureFile = colorTexture.c_str();
				textureListChannelOne[i] = new Texture(textureFile);
				textureListChannelOne[i]->LoadTexture(GL_TEXTURE1);
			}
		}

		textureListChannelThree[i] = nullptr;
		if (specularTexture != "") {
			if (material->GetTextureCount(aiTextureType_SPECULAR)) {
				aiString path;
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
					int idx = std::string(path.data).rfind("\\");
					std::string filename = std::string(path.data).substr(idx + 1);

					std::string texPath = std::string("Textures/") + filename;

					textureListChannelThree[i] = new Texture(texPath.c_str());

					if (!textureListChannelThree[i]->LoadTexture(GL_TEXTURE4)) {
						printf("Failed to load texture at: %s \n", texPath);
						delete textureListChannelThree[i];
						textureListChannelThree[i] = nullptr;
					}
				}
			}
			if (!textureListChannelThree[i]) {
				const char* textureFile = specularTexture.c_str();
				textureListChannelThree[i] = new Texture(textureFile);
				textureListChannelThree[i]->LoadTexture(GL_TEXTURE4);
			}
		}

		textureListChannelTwo[i] = nullptr;
		if(normalTexture != "") {
			if (material->GetTextureCount(aiTextureType_NORMALS)) {
				aiString path;
				if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
					int idx = std::string(path.data).rfind("\\");
					std::string filename = std::string(path.data).substr(idx + 1);

					std::string texPath = std::string("Textures/") + filename;

					textureListChannelTwo[i] = new Texture(texPath.c_str());

					if (!textureListChannelTwo[i]->LoadTexture(GL_TEXTURE5)) {
						printf("Failed to load texture at: %s \n", texPath);
						delete textureListChannelTwo[i];
						textureListChannelTwo[i] = nullptr;
					}
				}
			}
			if (!textureListChannelTwo[i]) {
				const char* textureFile = normalTexture.c_str();
				textureListChannelTwo[i] = new Texture(textureFile);
				textureListChannelTwo[i]->LoadTexture(GL_TEXTURE5);
			}
		}

		textureListChannelFour[i] = nullptr;
		if(heightTexture != "") {
			if (material->GetTextureCount(aiTextureType_HEIGHT)) {
				aiString path;
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS) {
					int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\'
					std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string

					std::string texPath = std::string("Textures/") + filename;

					textureListChannelFour[i] = new Texture(texPath.c_str());

					if (!textureListChannelFour[i]->LoadTexture(GL_TEXTURE7)) {
						printf("Failed to load texture at: %s \n", texPath);
						delete textureListChannelFour[i];
						textureListChannelFour[i] = nullptr; // can make a func for SAFE_DELETE( T& )
					}
				}
			}
			if (!textureListChannelFour[i]) {
				const char* textureFile = heightTexture.c_str();
				textureListChannelFour[i] = new Texture(textureFile);
				textureListChannelFour[i]->LoadTexture(GL_TEXTURE7);
			}
		}
	}
}












*/