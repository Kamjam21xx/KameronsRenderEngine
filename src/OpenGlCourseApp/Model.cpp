#include "Model.h"
#include <iostream>

// set flags for texture class and create singular function for loading texture 
// unstandardize textures and use rgb where no alpha channel is applicable -- rework shaders for vec3 

// not even using assimp materials, scene, or textures, finish obj loader and remove assimp from project


Model::Model()
{

}


 // restructure into one function with checks ---- same for others
 // keep for ease of changing for now

void Model::LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, bool tangents) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs 
												| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
												| aiProcess_CalcTangentSpace ); // complex bitflag shit, flippin ur bit flags. yeehaw

	if (!scene) {
		printf("Model failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene, drawType, tangents);

	LoadMaterials(scene, colorTexture);
}

void Model::LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, bool tangents) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs
		| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace); // complex bitflag shit, flippin ur bit flags. yeehaw

	if (!scene) {
		printf("Model failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene, drawType, tangents);

	LoadMaterials(scene, colorTexture, specularTexture);
}

void Model::LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, bool tangents) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs
		| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace ); // complex bitflag shit, flippin ur bit flags. yeehaw

	if (!scene) {
		printf("Model failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene, drawType, tangents);

	LoadMaterials(scene, colorTexture, specularTexture, normalTexture);
}

void Model::LoadModel(const std::string& fileName, GLenum drawType, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture, bool tangents) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs
		| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace); // complex bitflag shit, flippin ur bit flags. yeehaw

	if (!scene) {
		printf("Model failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene, drawType, tangents);

	LoadMaterials(scene, colorTexture, specularTexture, normalTexture, heightTexture);
}

void Model::RenderModel() {
	for (size_t i = 0; i < meshList.size(); ++i) {
		unsigned int materialIndex = meshToTex[i];
		
		if (materialIndex < textureListDiffuse.size() && textureListDiffuse[materialIndex]) {
			textureListDiffuse[materialIndex]->UseTexture();
		}
		if (materialIndex < textureListSpecular.size() && textureListSpecular[materialIndex]) {
			textureListSpecular[materialIndex]->UseTexture();
		}
		if (materialIndex < textureListNormal.size() && textureListNormal[materialIndex]) {
			textureListNormal[materialIndex]->UseTexture();
		}
		if (materialIndex < textureListHeight.size() && textureListHeight[materialIndex]) {
			textureListHeight[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel() { // will need to be revised with a pointer count pointing to what we are deleting and the pointers

	// throw in single loop when restructured 
	// make multithreaded

	for (size_t i = 0; i < meshList.size(); ++i) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	for (size_t i = 0; i < textureListDiffuse.size(); ++i) {
		if (textureListDiffuse[i]) {
			delete textureListDiffuse[i];
			textureListDiffuse[i] = nullptr;
		}
	}
	for (size_t i = 0; i < textureListSpecular.size(); ++i) {
		if (textureListSpecular[i]) {
			delete textureListSpecular[i];
			textureListSpecular[i] = nullptr;
		}
	}
	for (size_t i = 0; i < textureListHeight.size(); ++i) {
		if (textureListHeight[i]) {
			delete textureListSpecular[i];
			textureListHeight[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode *node, const aiScene *scene, GLenum drawType, bool tangents) {
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, drawType, tangents);
	}

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		LoadNode(node->mChildren[i], scene, drawType, tangents);
	}
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene, GLenum drawType, bool tangents) {
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
		newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size(), drawType);
	}
	else {
		newMesh->CreateMeshNoTangents(&vertices[0], &indices[0], vertices.size(), indices.size(), drawType);
	}
	
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *scene) {
	textureListDiffuse.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListDiffuse[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;
				
				textureListDiffuse[i] = new Texture(texPath.c_str());

				if (!textureListDiffuse[i]->LoadTextureA()) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListDiffuse[i];
					textureListDiffuse[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListDiffuse[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			textureListDiffuse[i] = new Texture("Textures/gridtexture.png"); // default material
			textureListDiffuse[i]->LoadTextureA();
		}
	}

}

void Model::LoadMaterials(const aiScene *scene, std::string colorTexture) {
	textureListDiffuse.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListDiffuse[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListDiffuse[i] = new Texture(texPath.c_str());

				if (!textureListDiffuse[i]->LoadTextureA()) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListDiffuse[i];
					textureListDiffuse[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListDiffuse[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = colorTexture.c_str();
			textureListDiffuse[i] = new Texture(textureFile);
			textureListDiffuse[i]->LoadTextureA();
		}
	}
}

void Model::LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture) {
	textureListDiffuse.resize(scene->mNumMaterials);
	textureListSpecular.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListDiffuse[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListDiffuse[i] = new Texture(texPath.c_str());

				if (!textureListDiffuse[i]->LoadTextureA()) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListDiffuse[i];
					textureListDiffuse[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}

		if (!textureListDiffuse[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = colorTexture.c_str();
			textureListDiffuse[i] = new Texture(textureFile);
			textureListDiffuse[i]->LoadTextureA();
		}
		textureListSpecular[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_SPECULAR)) {
			aiString path;
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListSpecular[i] = new Texture(texPath.c_str());

				if (!textureListSpecular[i]->LoadTextureA(GL_TEXTURE4)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListSpecular[i];
					textureListSpecular[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListSpecular[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = specularTexture.c_str();
			textureListSpecular[i] = new Texture(textureFile);
			textureListSpecular[i]->LoadTextureA(GL_TEXTURE4);
		}
	}
}

void Model::LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture) {
	textureListDiffuse.resize(scene->mNumMaterials);
	textureListSpecular.resize(scene->mNumMaterials);
	textureListNormal.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListDiffuse[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListDiffuse[i] = new Texture(texPath.c_str());

				if (!textureListDiffuse[i]->LoadTextureA()) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListDiffuse[i];
					textureListDiffuse[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListDiffuse[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = colorTexture.c_str();
			textureListDiffuse[i] = new Texture(textureFile);
			textureListDiffuse[i]->LoadTextureA();
		}

		textureListSpecular[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_SPECULAR)) {
			aiString path;
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListSpecular[i] = new Texture(texPath.c_str());

				if (!textureListSpecular[i]->LoadTextureA(GL_TEXTURE4)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListSpecular[i];
					textureListSpecular[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListSpecular[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = specularTexture.c_str();
			textureListSpecular[i] = new Texture(textureFile);
			textureListSpecular[i]->LoadTextureA(GL_TEXTURE4);
		}

		textureListNormal[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_NORMALS)) {
			aiString path;
			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListNormal[i] = new Texture(texPath.c_str());

				if (!textureListNormal[i]->LoadTextureNormalRGBA(GL_TEXTURE5)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListNormal[i];
					textureListNormal[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListNormal[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = normalTexture.c_str();
			textureListNormal[i] = new Texture(textureFile);
			textureListNormal[i]->LoadTextureNormalRGBA(GL_TEXTURE5);
		}
	}
}

void Model::LoadMaterials(const aiScene *scene, std::string colorTexture, std::string specularTexture, std::string normalTexture, std::string heightTexture) {
	textureListDiffuse.resize(scene->mNumMaterials);
	textureListSpecular.resize(scene->mNumMaterials);
	textureListNormal.resize(scene->mNumMaterials);
	textureListHeight.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial *material = scene->mMaterials[i];

		textureListDiffuse[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListDiffuse[i] = new Texture(texPath.c_str());

				if (!textureListDiffuse[i]->LoadTextureA()) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListDiffuse[i];
					textureListDiffuse[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListDiffuse[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = colorTexture.c_str();
			textureListDiffuse[i] = new Texture(textureFile);
			textureListDiffuse[i]->LoadTextureA();
		}

		textureListSpecular[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_SPECULAR)) {
			aiString path;
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListSpecular[i] = new Texture(texPath.c_str());

				if (!textureListSpecular[i]->LoadTextureA(GL_TEXTURE4)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListSpecular[i];
					textureListSpecular[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListSpecular[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = specularTexture.c_str();
			textureListSpecular[i] = new Texture(textureFile);
			textureListSpecular[i]->LoadTextureA(GL_TEXTURE4);
		}

		textureListNormal[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_NORMALS)) {
			aiString path;
			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListNormal[i] = new Texture(texPath.c_str());

				if (!textureListNormal[i]->LoadTextureNormalRGBA(GL_TEXTURE5)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListNormal[i];
					textureListNormal[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListNormal[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = normalTexture.c_str();
			textureListNormal[i] = new Texture(textureFile);
			textureListNormal[i]->LoadTextureNormalRGBA(GL_TEXTURE5);
		}

		textureListHeight[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_HEIGHT)) {
			aiString path;
			if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\"); // note worthy: reverse find to '\' 
				std::string filename = std::string(path.data).substr(idx + 1);// from '\' to the end ==  sub-string : good simple logic

				std::string texPath = std::string("Textures/") + filename;

				textureListHeight[i] = new Texture(texPath.c_str());

				if (!textureListHeight[i]->LoadTextureA(GL_TEXTURE7)) { // structure different for LoadTextureA() aswell
					printf("Failed to load texture at: %s \n", texPath);
					delete textureListHeight[i];
					textureListHeight[i] = nullptr; // can make a func for SAFE_DELETE( T& )
				}
			}
		}
		if (!textureListHeight[i]) { // C:\Users\Kameron\source\repos\OpenGlCourseApp\OpenGlCourseApp\Textures personal direct path
			const char* textureFile = heightTexture.c_str();
			textureListHeight[i] = new Texture(textureFile);
			textureListHeight[i]->LoadTextureA(GL_TEXTURE7);
		}
	}
}

// add metal

Model::~Model()
{
	// clean up all data via for loops at pointers
}
