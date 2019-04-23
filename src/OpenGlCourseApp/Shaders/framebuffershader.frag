#version 430

in vec2 TexCoords;

out vec4 FragColor;

layout (binding = 16) uniform sampler2D screenSpaceTexture;

uniform float gammaLevel;

const float offset = 1.0 / 300.0;

// create new uniform in the shader class for the screenspace texture

void main() 
{
	vec3 hdrColor = texture(screenSpaceTexture, TexCoords).xyz;

	// reinhards tone mapping 
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
	// gamma correction
	mapped = pow(mapped, vec3(1.0f / gammaLevel));

	FragColor = vec4(mapped, 1.0f);
}