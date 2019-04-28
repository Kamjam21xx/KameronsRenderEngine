#version 430

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

layout (binding = 18) uniform sampler2D screenSpaceTexture;
layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;
// setup secondary fragment shader and setup shaderHandler for this

// uniform float gammaLevel;

// const float offset = 1.0 / 300.0;

void main() 
{
	vec3 hdrColor = vec3(0.0f, 0.0f, 0.0f);
	if(TexCoords.x < 0.5f)
	{
		hdrColor = texture(screenSpaceTexture, TexCoords).xyz;
	}
	else
	{
		hdrColor = texture(screenSpaceTextureTwo, TexCoords).xyz;
	}
	// reinhards tone mapping 
	// vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	// gamma correction
	// mapped = pow(mapped, vec3(1.0f / gammaLevel));

	FragColor = vec4(hdrColor, 1.0f);
}