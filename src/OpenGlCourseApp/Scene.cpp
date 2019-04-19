#include "Scene.h"


Scene::Scene()
{

}

Scene::Scene(std::string newFilePath) {
	sceneFileLocation = newFilePath;
}
void Scene::load() 
{
	if (sceneFileLocation == "" || NULL)
	{
		printf("Scene.sceneFileLocation not set : cannot load");
	}
	else
	{
		loadSceneFile();
	}
}
void Scene::loadNewFileLocation(std::string newFilePath) 
{
	sceneFileLocation = newFilePath;
	loadSceneFile();
}

void Scene::loadSceneFile() { // pretty much has to be a full function. dont break it up
	
	try {
		std::ifstream file(sceneFileLocation);
		if (file.is_open()) {
			std::cout << "default.FLS loading \n";
		}
		else {
			std::cout << "Scene.load() failed to open file stream \n";
		}

		std::string location, model, accessFlag, color, specular, normal, height, path, bitFlagStr;
		GLenum drawType;
		bool calcTangents;
		std::bitset<8> bitFlags; 
		short int arrayLocation = 0;

		while (file >> location >> model >> bitFlagStr) {

			std::reverse(bitFlagStr.begin(), bitFlagStr.end());
			bitFlags = std::bitset<8>(bitFlagStr);
			if (bitFlags[0] == 1) { continue; }

			path = location + model;

			switch (bitFlags[2]) { // DRAW TYPE
				case (1) : drawType = GL_STATIC_DRAW;
						   break;
				case (0) : if (bitFlags[3] == 1) {
						       drawType = GL_DYNAMIC_DRAW;
							   break;
						   }
						   else {
						       drawType = GL_STREAM_DRAW;
							   break;
						   }
			}
			switch (bitFlags[1]) { // ENABLE TANGENTS
				case (1) : calcTangents = true;
					       break;
				case (0) : calcTangents = false;
					       break;
			}
			switch (bitFlags[4]) { // COLOR TEXTURE
				case (1) : color = path + "_COLOR.png";
					       break;
				case (0) : color = "";
					       break;

			}
			switch (bitFlags[5]) { // SPECULAR TEXTURE
				case (1) : specular = path + "_SPECULAR.png";
					       break;
				case (0) : specular = "";
					       break;
			}
			switch (bitFlags[6]) { // NORMAL TEXTURE
				case (1) : normal = path + "_NORMAL.png";
					       break;
				case (0) : normal = "";
					       break;
			}
			switch (bitFlags[7]) { // HEIGHT TEXTURE
				case (1) : height = path + "_HEIGHT.png";
					       break;
				case (0) : height = "";
					       break;
			}

			// PUSH and LOAD OBJECT
			objects.push_back(Model());
			objects[arrayLocation].LoadModel(
				path + ".fbx",
				GL_STREAM_DRAW,
				color,
				specular,
				normal,
				height,
				bitFlags
			);
			arrayLocation++;	
		}
	
		if (file.is_open()) {
			file.close();
		}
		std::cout << "default.FLS closed\n";
	}
	catch (std::exception& exc) {
		std::cout << "Scene.load() interuprted while loading \n";
	}
}

void Scene::render() {
	
}



	// rework load() with producer consumer in 2 threads

	// possibly add logging boolean

	// will add bitflag storage for model class
	// bitflags will be used for rendering order and shader use

		/*       bitset layout is as follows
				

			0	ignore 

			1	calc tangents

			2	draw type(2 bits)
			3

			4	color

			5	specular

			6	normal

			7	height
		*/
	// change bitset to sized int

Scene::~Scene()
{

}
