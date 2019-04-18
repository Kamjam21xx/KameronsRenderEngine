#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <bitset>
#include <algorithm>

#include "Model.h"



class Scene
{
public:
	// possibly add skybox , keep loading functionality for the skybox a member of itself
	std::vector <Model> objects;	
	
	Scene();
	Scene(std::string sceneFileLocation);

	void load();
	void render();

	~Scene();

private:
	


	std::string sceneFile;

	
};

