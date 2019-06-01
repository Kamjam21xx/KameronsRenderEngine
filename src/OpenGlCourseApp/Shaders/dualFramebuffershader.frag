#version 430

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

layout (binding = 18) uniform sampler2D screenSpaceTexture; // color
layout (binding = 19) uniform sampler2D screenSpaceTextureTwo; // bloom

uniform float bloomStrength;

void main() 
{
	// color + bloom * bloom strength
	FragColor = vec4(texture(screenSpaceTexture, TexCoords).xyz + texture(screenSpaceTextureTwo, TexCoords).xyz * bloomStrength, 1.0f);
}	






// reinhards tone mapping 
	// vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	// gamma correction
	// mapped = pow(mapped, vec3(1.0f / gammaLevel));