#include "Shader.h"
#include <thread>

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) 
{
	CompileShader(vertexCode, fragmentCode);
}
void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) 
{
	std::string vertexString;
	std::string fragmentString;

	vertexString = ReadFile(vertexLocation);
	fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShader(vertexCode, fragmentCode);
}
void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString;
	std::string fragmentString;
	std::string geometryString;

	vertexString = ReadFile(vertexLocation);
	fragmentString = ReadFile(fragmentLocation);
	geometryString = ReadFile(geometryLocation);


	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShader(vertexCode, geometryCode, fragmentCode);
}
std::string Shader::ReadFile(const char* fileLocation) 
{
	std::string content;

	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.");
		content = "";
	}
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	return content;
}
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) 
{
	shaderID = glCreateProgram();
	if (!shaderID) {
		printf("Error creating shader program");
		return;
	}
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	
	CompileProgram();
}
void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();
	if (!shaderID) {
		printf("Error creating shader program");
		return;
	}
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();

	glDeleteShader(shaderID);
}
void Shader::Validate() const
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s' \n", eLog);
		return;
	}
}
void Shader::CompileProgram() 
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s' \n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformSpecularPower = glGetUniformLocation(shaderID, "material.specularPower");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformEyeDirection = glGetUniformLocation(shaderID, "eyeDirection");
	uniformSplitScreenIsOn = glGetUniformLocation(shaderID, "splitScreenIsOn");
	uniformSplitScreenType = glGetUniformLocation(shaderID, "splitScreenType");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i != MAX_POINT_LIGHTS; ++i) {
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.colour", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.diffusionIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i != MAX_SPOT_LIGHTS; ++i) {
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.colour", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.diffusionIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformTextureDiffuse = glGetUniformLocation(shaderID, "theTextureDiffuse");
	uniformTextureSpecular = glGetUniformLocation(shaderID, "theTextureSpecular");
	uniformTextureMetal = glGetUniformLocation(shaderID, "theTextureMetal");
	uniformTextureNormal = glGetUniformLocation(shaderID, "theTextureNormal");
	uniformTextureHeight = glGetUniformLocation(shaderID, "theTextureHeight");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
	uniformTextureSkyBox = glGetUniformLocation(shaderID, "skyBoxTexture");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}

	for (size_t i = 0; i < (MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS); i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTextureScreenSpace = glGetUniformLocation(shaderID, "screenSpaceTexture");
	
}

GLuint Shader::GetDiffuseIntensityLocation() const
{
	return uniformDirectionalLight.uniformDiffuseIntensity;		
}
GLuint Shader::GetDirectionLocation() const
{	
	return uniformDirectionalLight.uniformDirection;			
}
GLuint Shader::GetProjectionLocation() const
{	
	return uniformProjection;									
}
GLuint Shader::GetModelLocation() const
{	
	return uniformModel;										
}
GLuint Shader::GetViewLocation() const
{	
	return uniformView;											
}		
GLuint Shader::GetAmbientColorLocation() const
{	
	return uniformDirectionalLight.uniformColor;				
}
GLuint Shader::GetAmbientIntensityLocation() const
{	
	return uniformDirectionalLight.uniformAmbientIntensity;		
}
GLuint Shader::GetSpecularIntensityLocation() const
{	
	return uniformSpecularIntensity;							
}
GLuint Shader::GetSpecularPowerLocation() const
{	
	return uniformSpecularPower;								
}
GLuint Shader::GetEyePositionLocation()	const
{	
	return uniformEyePosition;									
}
GLuint Shader::GetEyeDirectionLocation()const
{					 
	return uniformEyeDirection;
}
GLuint Shader::GetOmniLightLocation() const
{
	return uniformOmniLightPos;
}
GLuint Shader::GetFarPlanelocation() const
{
	return uniformFarPlane;
}
GLuint Shader::GetSplitScreenIsOnLocation() const
{
	return uniformSplitScreenIsOn;
}
GLuint Shader::GetSplitScreenTypeLocation() const
{
	return uniformSplitScreenType;
}
GLuint Shader::GetTextureScreenSpaceLocation() const
{
	return uniformTextureScreenSpace;
}

void Shader::SetDirectionalLight(DirectionalLight * dLight) 
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, 
					 uniformDirectionalLight.uniformColor,
		             uniformDirectionalLight.uniformDiffuseIntensity, 
					 uniformDirectionalLight.uniformDirection );
}
void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset) 
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount); 

	for (size_t i = 0; i < lightCount; i++) {
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity,
							uniformPointLight[i].uniformColor,
							uniformPointLight[i].uniformDiffuseIntensity,
							uniformPointLight[i].uniformPosition,
							uniformPointLight[i].uniformConstant,
							uniformPointLight[i].uniformLinear,
							uniformPointLight[i].uniformExponent
		);

		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}
void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount); 

	for (size_t i = 0; i < lightCount; i++) {
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity,
			uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition,
			uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant,
			uniformSpotLight[i].uniformLinear,
			uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge
		);

		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
	}
}
void Shader::SetTextureDiffuse(GLuint textureUnit) 
{ 
	glUniform1i(uniformTextureDiffuse, textureUnit);
}
void Shader::SetTextureSpecular(GLuint textureUnit)
{
	glUniform1i(uniformTextureSpecular, textureUnit);
}
void Shader::SetTextureMetal(GLuint textureUnit) 
{
	glUniform1i(uniformTextureMetal, textureUnit);
}
void Shader::SetTextureNormal(GLuint textureUnit)
{
	glUniform1i(uniformTextureNormal, textureUnit);
}
void Shader::SetTextureHeight(GLuint textureUnit) {
	glUniform1i(uniformTextureHeight, textureUnit);
}
void Shader::SetDirectionalShadowMap(GLuint textureUnit) 
{ 
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}
void Shader::SetDirectionalLightTransform(glm::mat4 *lTransform) 
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}
void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) 
{
	for (size_t i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}
void Shader::SetTextureSkyBox(GLuint textureUnit) 
{
	glUniform1i(uniformTextureSkyBox, textureUnit);
}
void Shader::SetSplitScreenIsOn(GLboolean splitScreenIsOn) 
{
	glUniform1i(uniformSplitScreenIsOn, splitScreenIsOn);
}
void Shader::SetSplitScreenType(GLuint splitScreenType) 
{
	glUniform1i(uniformSplitScreenType, splitScreenType);
}
void Shader::SetTextureScreenSpace(GLuint textureUnit)
{
	glUniform1i(uniformTextureScreenSpace, textureUnit);
}

void Shader::UseShader() 
{	
	glUseProgram(shaderID);	
}
void Shader::ClearShader() 
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);
	
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s' \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}
