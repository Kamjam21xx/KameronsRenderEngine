#version 430

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

layout (binding = 18) uniform sampler2D screenSpaceTexture;
layout (binding = 19) uniform sampler2D screenSpaceTextureTwo;

uniform float brightness;
uniform float contrast;
uniform float saturation;

mat4 CalcMatriceBCS()
{
	vec3 luminance = vec3(0.3086f, 0.6094f, 0.0820f);
	float iS = 1.0f - saturation;
	float T = (brightness * contrast) + (1.0f - contrast / 2);

	vec3 r = vec3(luminance.r * iS);
	vec3 g = vec3(luminance.g * iS);
	vec3 b = vec3(luminance.b * iS);

	r.r += saturation;
	g.g += saturation;
	b.b += saturation;

	float BCS30 = T * r.x + T * g.x + T * b.x;
	float BCS31 = T * r.y + T * g.y + T * b.y;
	float BCS32 = T * r.z + T * g.z + T * b.z;

	float c = contrast;
	mat4 BCS = mat4(	c * r.x,		c * r.y,		c * r.z,		0,
						c * g.x,		c * g.y,		c * g.z,		0,
						c * b.x,		c * b.y,		c * b.z,		0,
						BCS30,			 BCS31,			BCS32,			1);

	return BCS;
}

void main() 
{
	vec3 hdrColor = texture(screenSpaceTexture, TexCoords).xyz;
	hdrColor += texture(screenSpaceTextureTwo, TexCoords).xyz;

	FragColor = vec4(hdrColor, 1.0f);
	FragColor = CalcMatriceBCS() * FragColor;
}	






// reinhards tone mapping 
	// vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	// gamma correction
	// mapped = pow(mapped, vec3(1.0f / gammaLevel));