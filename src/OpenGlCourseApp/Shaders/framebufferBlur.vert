#version 430

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tex;

out vec2 TexCoords;
out vec2 pTexelSize1;
out vec2 mTexelSize1;
out vec2 pTexelSize2;
out vec2 mTexelSize2;
out vec2 pTexelSize3;
out vec2 mTexelSize3;
out vec2 pTexelSize4;
out vec2 mTexelSize4;

layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;

uniform vec2 blurDirection;

void main() 
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);

	TexCoords = tex;

	vec2 TexelSize = 1.0f / textureSize(screenSpaceTextureTwo, 0);
	TexelSize = vec2(TexelSize.x * blurDirection.x, TexelSize.y * blurDirection.y);

	pTexelSize1 = TexCoords + TexelSize;
	mTexelSize1 = TexCoords - TexelSize;
	pTexelSize2 = TexCoords + TexelSize * 2.0;	
	mTexelSize2 = TexCoords - TexelSize * 2.0;	
	pTexelSize3 = TexCoords + TexelSize * 3.0;	
	mTexelSize3 = TexCoords - TexelSize * 3.0;	
	pTexelSize4 = TexCoords + TexelSize * 4.0;
	mTexelSize4 = TexCoords - TexelSize * 4.0;
}