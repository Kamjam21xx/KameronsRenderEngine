#version 430

in vec2 TexCoords;

out vec4 FragColor;

layout (binding = 16) uniform sampler2D screenSpaceTexture;

const float offset = 1.0 / 300.0;

// create new uniform in the shader class for the screenspace texture

void main() 
{
	FragColor = vec4(0.0,0.0,0.0,0.0);
	FragColor.xyz = texture(screenSpaceTexture, TexCoords).xyz;
}