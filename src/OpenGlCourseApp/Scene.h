#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <bitset>
#include <algorithm>

#include "Model.h"
#include "SkyBox.h"


class Scene
{
public:
	// possibly add skybox , keep loading functionality for the skybox a member of itself
	SkyBox skybox;

	std::vector <Model> objects;	
	
	Scene();
	Scene(std::string newFilePath);

	void load();
	void loadNewFileLocation(std::string newFilePath);
	void loadSceneFile();
	void render();

	SkyBox* GetSkyBoxPtr();

	~Scene();

private:
	
	std::string sceneFileLocation;
	

};

