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

	void Start();
	void End();
	void DisplayInfo();
	
	void EditLights(PointLight *pLights, SpotLight *sLights, DirectionalLight *dLight, unsigned short int pLightCount, unsigned short int sLightCount, bool dLightEdit, bool pLightEdit, bool sLightEdit);
	void EditDLight(DirectionalLight *dLight);
	void EditPLights(PointLight *pLights, unsigned short int lightCount);
	void EditSLights(SpotLight *sLights, unsigned short int lightCount);

	void EditScene(GLfloat *spin);
	void EditSceneSpin(GLfloat *spin);

	void EditRenderSettings(GLboolean *splitScreenIsOn, GLuint *splitScreenType, GLfloat *gamma);
	void EditFiltering();
	void EditGamma(GLfloat *gamma);
	void EditVerticalSync();
	void EditSplitScreen(GLboolean *splitScreenIsOn, GLuint *splitScreenType);


	~GraphicUI();

private:
	
	GLFWwindow *window;

	bool enableSpin, enableVerticalSync, enableEditFiltering, enableGammaEdit;

	GLint filteringLevel;
	GLfloat stateSaveSpin;

	// possible take addresses per function call
	// OR store addresses for v-table lookups
	// OOOR make optional to use both methods
};

