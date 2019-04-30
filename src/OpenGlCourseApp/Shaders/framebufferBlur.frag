#version 430

// implement graphites tips at a later date for super speedy and manipulatable code

in vec2 TexCoords;
in vec2 TexelSize;

out vec4 FragColor;

layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;

// uniforms variables
uniform bool isHorizontal;
// float weights[5] = float [] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);// can use

vec4 GaussianBlur(vec3 inputColor, vec2 stepSize) // bake shaders for different resolutions so that multiplication doesnt need to be uselessly done in the shader for every pixel
{
// could do stepsizes in the vertex shader.... idk
	if(isHorizontal)
	{
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(stepSize.x * 1, 0.0f)).rgb * 0.1945946f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(stepSize.x * 1, 0.0f)).rgb * 0.1945946f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(stepSize.x * 2, 0.0f)).rgb * 0.1216216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(stepSize.x * 2, 0.0f)).rgb * 0.1216216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(stepSize.x * 3, 0.0f)).rgb * 0.054054f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(stepSize.x * 3, 0.0f)).rgb * 0.054054f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(stepSize.x * 4, 0.0f)).rgb * 0.016216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(stepSize.x * 4, 0.0f)).rgb * 0.016216f;

	}
	else
	{

		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(0.0f, stepSize.y * 1)).rgb * 0.1945946f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(0.0f, stepSize.y * 1)).rgb * 0.1945946f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(0.0f, stepSize.y * 2)).rgb * 0.1216216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(0.0f, stepSize.y * 2)).rgb * 0.1216216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(0.0f, stepSize.y * 3)).rgb * 0.054054f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(0.0f, stepSize.y * 3)).rgb * 0.054054f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords + vec2(0.0f, stepSize.y * 4)).rgb * 0.016216f;
		inputColor += texture(screenSpaceTextureTwo, TexCoords - vec2(0.0f, stepSize.y * 4)).rgb * 0.016216f;

	}

	return vec4(inputColor, 1.0f);
}


void main() 
{
	vec3 highlightSample = texture(screenSpaceTextureTwo, TexCoords).xyz * 0.227027f; // doesnt work if []
	
	FragColor = GaussianBlur(highlightSample, TexelSize);
}