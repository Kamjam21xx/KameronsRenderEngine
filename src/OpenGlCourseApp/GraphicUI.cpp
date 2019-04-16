#include "GraphicUI.h"



GraphicUI::GraphicUI(GLFWwindow *windowPtr)
{
	window = windowPtr;
}


void GraphicUI::Start() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


}
void GraphicUI::End() {

		ImGui::EndFrame();
		ImGui::Render();
		glfwMakeContextCurrent(window);
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
}
void GraphicUI::DisplayInfo() {
		ImGui::Begin("Window Information");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
}

void GraphicUI::EditLights(PointLight *pLights,SpotLight *sLights, DirectionalLight *dLight, unsigned short int pLightCount, unsigned short int sLightCount, bool dLightEdit, bool pLightEdit, bool sLightEdit) { // shitty setup, change later
	
	// rework checks to || "or" to increase execution speed.

	ImGui::Begin("Light Editor");
	if (dLightEdit && (dLight != NULL)) {
		ImGui::Text("Directional Light");
		EditDLight(dLight);
	}
	if (pLightEdit && (pLights != NULL) && (pLightCount != NULL) && (pLightCount >= 1)) {
		ImGui::Text("Point Lights");
		EditPLights(pLights, pLightCount);
	}
	if (sLightEdit && (sLights != NULL) && (sLightCount != NULL) && (sLightCount >= 1)) {
		ImGui::Text("Spot Lights");
		EditSLights(sLights, sLightCount);
	}
	ImGui::End();
}
void GraphicUI::EditDLight(DirectionalLight *dLight) {

	// Set variables
	float dLightDiffuse = dLight->GetDiffuseIntensity();
	float dLightAmbient = dLight->GetAmbientIntensity();
	ImVec4 dLightColor = ImVec4(dLight->GetLightColorRed(), dLight->GetLightColorBlue(), dLight->GetLightColorGreen(), 1.0f);

	// make GUI
	ImGui::SliderFloat("Diffuse", &dLightDiffuse, 0.0f, 2.0f);
	ImGui::SliderFloat("Ambient", &dLightAmbient, 0.0f, 0.2f);
	ImGui::ColorEdit3("Color", (float*)&dLightColor);

	// Set member values to variable values
	dLight->SetDiffuseIntensity(dLightDiffuse);
	dLight->SetAmbientIntensity(dLightAmbient);
	dLight->SetLightColor(dLightColor.x, dLightColor.y, dLightColor.z);
}
void GraphicUI::EditPLights(PointLight *pLights, unsigned short int lightCount) {

	for (unsigned int i = 0; i < lightCount; ++i) {
		ImGui::PushID(i);

		// Set variables
		float pLightDiffuse = pLights[i].GetDiffuseIntensity();
		float pLightAmbient = pLights[i].GetAmbientIntensity();
		float pLightRange = pLights[i].GetRange();
		ImVec4 pLightColor = ImVec4(pLights[i].GetLightColorRed(), pLights[i].GetLightColorGreen(), pLights[i].GetLightColorBlue(), 1.00f);

		// make GUI
		ImGui::Text(" ");
		ImGui::SliderFloat("Diffuse", &pLightDiffuse, 0.0f, 20.0f);
		ImGui::SliderFloat("Ambient", &pLightAmbient, 0.0f, 0.2f);
		ImGui::SliderFloat("Range", &pLightRange, 0.0f, 100.0f);
		ImGui::ColorEdit3("Color", (float*)&pLightColor);

		// Set member values to variable values
		pLights[i].SetDiffuseIntensity(pLightDiffuse);
		pLights[i].SetAmbientIntensity(pLightAmbient);
		pLights[i].SetLightRange(pLightRange);
		pLights[i].SetLightColor(pLightColor.x, pLightColor.y, pLightColor.z);

		ImGui::PopID();
	}
}
void GraphicUI::EditSLights(SpotLight *sLights, unsigned short int lightCount) {
	
	for (unsigned int i = 0; i < lightCount; ++i) {
		ImGui::PushID(i);

		// Set variables
		float pLightDiffuse = sLights[i].GetDiffuseIntensity();
		float pLightAmbient = sLights[i].GetAmbientIntensity();
		float pLightRange = sLights[i].GetRange();
		ImVec4 pLightColor = ImVec4(sLights[i].GetLightColorRed(), sLights[i].GetLightColorGreen(), sLights[i].GetLightColorBlue(), 1.00f);

		// make GUI
		ImGui::Text(" ");
		ImGui::SliderFloat("Diffuse", &pLightDiffuse, 0.0f, 20.0f);
		ImGui::SliderFloat("Ambient", &pLightAmbient, 0.0f, 0.2f);
		ImGui::SliderFloat("Range", &pLightRange, 0.0f, 100.0f);
		ImGui::ColorEdit3("Color", (float*)&pLightColor);

		// Set member values to variable values
		sLights[i].SetDiffuseIntensity(pLightDiffuse);
		sLights[i].SetAmbientIntensity(pLightAmbient);
		sLights[i].SetLightRange(pLightRange);
		sLights[i].SetLightColor(pLightColor.x, pLightColor.y, pLightColor.z);

		ImGui::PopID();
	}
}

void GraphicUI::EditScene(GLfloat *spin) {
	
	ImGui::Begin("Scene Editor");

	EditSceneSpin(spin);

	ImGui::End();
}
void GraphicUI::EditSceneSpin(GLfloat *spin) {
	
	if (ImGui::Button("Enable Spin")) {
		
		enableSpin = !enableSpin;

		if (enableSpin) {

			(*spin) = stateSaveSpin;
		}
		else {
			stateSaveSpin = (*spin);
			(*spin) = 0.0f;
		}
	}

	if (enableSpin) {
		ImGui::SliderFloat("Spin", spin, -1.0f, 1.0f);
	}
	else {

	}

}

GraphicUI::~GraphicUI()
{

}

/*
	static int counter = 0;
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
*/