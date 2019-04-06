#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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

