#version 430

in vec3 TexCoords;

layout (location = 0) out vec4 colour;
layout (location = 1) out vec4 highlights;

uniform samplerCube skybox;

uniform float gammaLevel;
uniform float bloomThreshold;

vec4 GetHighlights(vec4 colorSample)
{
	vec4 sampleColor = colorSample;

	float brightness = dot(colorSample.rgb, vec3(0.2126f, 0.7512f, 0.0722f));

	vec4 highlightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	if(brightness > bloomThreshold)
	{
		highlightColor = colorSample;
	}
	
	return vec4(highlightColor.xyz, 1.0f);
}
vec4 ApplyGammaToneMapping(vec3 hdrColor)
{
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
	mapped = pow(mapped, vec3(1.0f / gammaLevel));

	return vec4(mapped, 1.0f);
}

void main()
{
	colour = texture(skybox, TexCoords);
	colour = ApplyGammaToneMapping(colour.xyz);

	highlights = GetHighlights(colour);
}