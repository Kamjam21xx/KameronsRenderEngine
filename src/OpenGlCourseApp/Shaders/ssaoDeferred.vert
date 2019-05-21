#version 430

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tex;

out vec2 TexCoord;
//out float TexelSize;

void main() 
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);

	TexCoord = tex;

	//TexelSize = 1.0f / textureSize(screenSpaceTextureTwo, 0);
}