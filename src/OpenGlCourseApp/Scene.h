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
	Scene();
	Scene(std::string sceneFileLocation);

	void load();
	void render();





	~Scene();

	std::vector <Model> objects;

private:
	std::string sceneFile;

};

