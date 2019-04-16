#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <stdio.h>
#include <iostream>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class GraphicUI
{
public:
	
	GraphicUI(GLFWwindow *windowPtr);

	void Render();
	void DisplayInfo();
	
	void EditLights(std::vector<PointLight> *pLights, std::vector<SpotLight> *sLights, DirectionalLight *dLight, unsigned short int pLightCount, unsigned short int sLightCount);
	void EditDLight(DirectionalLight *dLight);
	void EditPLights(std::vector<PointLight> *pLights, unsigned short int lightCount);
	void EditSLights(std::vector<SpotLight> *sLights, unsigned short int lightCount);



	~GraphicUI();

private:
	
	GLFWwindow *window;

	// possible take addresses per function call
	// OR store addresses for v-table lookups
	// OOOR make optional to use both methods
};

