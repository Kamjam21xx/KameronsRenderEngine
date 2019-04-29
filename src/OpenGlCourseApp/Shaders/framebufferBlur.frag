#version 430

in vec2 TexCoords;

layout (location = 1) out vec4 Result;

layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;

// uniforms variables
uniform bool isHorizontal;
uniform float weights[5] = float [] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


vec4 SeperatedGaussianBlur(vec3 inputColor, vec2 stepSize)
{
	if(isHorizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			inputColor += texture(image, TexCoords + vec2(stepSize.x * i, 0.0)).rgb * weights[i];
			inputColor += texture(image, TexCoords - vec2(stepSize.x * i, 0.0)).rgb * weights[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			inputColor += texture(image, TexCoords + vec2(stepSize.y * i, 0.0)).rgb * weights[i];
			inputColor += texture(image, TexCoords - vec2(stepSize.y * i, 0.0)).rgb * weights[i];
		}
	}

	return vec4(inputColor, 1.0f);
}
vec2 GetTexelSize()
{
	return 1.0f / textureSize(screeSpaceTextureTwo, 0);
}

void main() 
{
	// get the size of a texel to use as an offset to TexCoords, for sampling.
	vec2 texelSize = GetTexelSize();

	// get initial value to work on.
	vec3 highlightSample = texture(screenSpaceTexture, TexCoords).xyz * weight[0];
	
	Result = SeperatedGaussianBlur(highlightSample, texelSize);
}