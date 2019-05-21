#version 430

in vec3 TexCoords;

layout (location = 0) out vec4 colour;
layout (location = 1) out vec4 highlights;

uniform samplerCube skybox;

uniform float gammaLevel;
uniform float bloomThreshold;
uniform float brightness;
uniform float contrast;
uniform float saturation;

vec4 CalcBrightnessContrastSaturation(vec4 Colour){
    float b = brightness;
    float c = contrast;
    float s = saturation;
    
    float t = c * -.5 + .5;
    vec3 l = vec3( .3086, .6094, .0820 );
    vec3 f = (-s * l + l) * c;
    vec3 g = s * c + f;

    mat4 m = mat4(
        g.r, f.r, f.r, 0,
        f.g, g.g, f.g, 0,
        f.b, f.b, g.b, 0,
        vec3(b + t), 1
    );

	return vec4(m[0].rgg * Colour.r + (m[1].rgr * Colour.g + (m[2].rb * Colour.b + m[3].x).rrg), 1.0f); 
}
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
	colour = CalcBrightnessContrastSaturation(colour);
	highlights = GetHighlights(colour);
}