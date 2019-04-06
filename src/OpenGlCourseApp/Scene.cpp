#include "Scene.h"



Scene::Scene()
{

}

Scene::Scene(std::string sceneFileLocation) {
	sceneFile = sceneFileLocation;
}


void Scene::load() {
	
	std::ifstream file("Models/default.FLS");
	if (file.is_open()) {
		std::cout << "default.FLS loading \n";
	}
	else {
		std::cout << "Scene.load() failed to open file stream \n";
	}


	std::string location, model, accessFlag;
	short int arrayLocation = 0;
	
	while (file >> location >> model >> accessFlag) {
		// load scene objects into scene array 

		// make automatic with file for models and checks

		objects.push_back(Model());
		if (accessFlag == "10") {
			objects[arrayLocation].LoadModel(
				location + model + ".fbx",								// Model
				GL_STATIC_DRAW,											// Draw Type
				location + model + "_COLOR.png",						// color texture disk location
				location + model + "_SPECULAR.png",					// Specular texture disk location
				location + model + "_NORMAL.png",					// normal texture			
				location + model + "_HEIGHT.png",
				1													// boolean calc tangents true or false for vertex layout										
			);
		}
		else if (accessFlag == "11") {
			objects[arrayLocation].LoadModel(
				location + model + ".fbx",
				GL_DYNAMIC_DRAW,
				location + model + "_COLOR.png",
				location + model + "_SPECULAR.png",
				location + model + "_NORMAL.png",
				location + model + "_HEIGHT.png",
				1
			);
		}
		else if (accessFlag == "00") {
			objects[arrayLocation].LoadModel(
				location + model + ".fbx",
				GL_STREAM_DRAW,
				location + model + "_COLOR.png",
				location + model + "_SPECULAR.png",
				location + model + "_NORMAL.png",
				location + model + "_HEIGHT.png",
				1
			);
		}
		else {
			std::cout << "ERROR invalid access flag \n";
		}
		arrayLocation++;
	}

	if (file.is_open()) {
		file.close();
	}
	std::cout << "default.FLS closed\n";
}

void Scene::render() {
	// ONLY CALL FROM MAIN RENDER FUNCTION
	// rework main and scene to avoid dancing between the two
}


Scene::~Scene()
{

}
