#version 430

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

layout (binding = 18) uniform sampler2D screenSpaceTexture;
layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;
// add secondary colorBuffer binding

uniform float gammaLevel;

const float offset = 1.0 / 300.0;

void main() 
{
	
	vec3 hdrColor = texture(screenSpaceTexture, TexCoords).xyz;
	
	
	
	
	FragColor = vec4(hdrColor, 1.0f);


}