#version 430

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

layout (binding = 18) uniform sampler2D screenSpaceTexture;
layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;


void main() 
{
	vec3 hdrColor = texture(screenSpaceTexture, TexCoords).xyz;
	hdrColor += texture(screenSpaceTextureTwo, TexCoords).xyz;

	FragColor = vec4(hdrColor, 1.0f);
}	






// reinhards tone mapping 
	// vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	// gamma correction
	// mapped = pow(mapped, vec3(1.0f / gammaLevel));