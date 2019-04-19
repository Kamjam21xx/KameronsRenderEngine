#version 430

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D theTextureDiffuse;

const float offset = 1.0 / 300.0;

// create new uniform in the shader class for the screenspace texture

void main() 
{
	FragColor = texture(theTextureDiffuse, TexCoords);
}