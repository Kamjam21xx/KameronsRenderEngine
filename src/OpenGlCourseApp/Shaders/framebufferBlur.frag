#version 430

// implement graphites tips at a later date for super speedy and manipulatable code

in vec2 TexCoords;
in vec2 pTexelSize1;
in vec2 mTexelSize1;
in vec2 pTexelSize2;
in vec2 mTexelSize2;
in vec2 pTexelSize3;
in vec2 mTexelSize3;
in vec2 pTexelSize4;
in vec2 mTexelSize4;

out vec4 FragColor;

layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;

void main() 
{
	vec3 inputColor = texture(screenSpaceTextureTwo, TexCoords).xyz * 0.227027f;

	inputColor += texture(screenSpaceTextureTwo, pTexelSize1).rgb * 0.1945946f;
	inputColor += texture(screenSpaceTextureTwo, mTexelSize1).rgb * 0.1945946f;
	inputColor += texture(screenSpaceTextureTwo, pTexelSize2).rgb * 0.1216216f;
	inputColor += texture(screenSpaceTextureTwo, mTexelSize2).rgb * 0.1216216f;
	inputColor += texture(screenSpaceTextureTwo, pTexelSize3).rgb * 0.054054f;
	inputColor += texture(screenSpaceTextureTwo, mTexelSize3).rgb * 0.054054f;
	inputColor += texture(screenSpaceTextureTwo, pTexelSize4).rgb * 0.016216f;
	inputColor += texture(screenSpaceTextureTwo, mTexelSize4).rgb * 0.016216f;

	FragColor = vec4(inputColor, 1.0);
}