#version 430

in vec2 TexCoord;

layout (location = 0) = SSAO;

layout (binding = 24) uniform sampler2D theTextureNoiseSSAO;
layout (binding = 23) uniform sampler2D theTextureDepth; // depth from eye to fragment
layout (binding = 21) uniform sampler2D screenSpaceTextureTwo; // normal and height map

uniform mat4 projection;
uniform mat4 inverseProjection;
uniform mat4 inverseView;
uniform vec3 samples[64]; // maybe bake this

const vec2 noiseScale = vec2(3840.0/4.0, 2160.0/4.0); // 1/4 screen resolution 1080p from 4k

vec3 CalcDepthToFragPos(float depth)
{
	float z = depth * 2.0f - 1.0f;

	vec4 clipSpacePos = vec4(TexCoord * 2.0f - 1.0f, z, 1.0f);
	vec4 viewSpacePos = inverseProjection * clipSpacePos; // simplify this

	viewSpacePos /= viewSpacePos.w;

	vec4 worldSpacePos = inverseView * viewSpacePos; // simplify this and return instead of assigning L value

	return worldSpacePos.xyz;
}
vec3 CalcDepthToViewFragPos(vec2 texCoords)
{
	float z = texture(theTextureDepth, texCoords);
	float x = texCoords.x * 2.0 - 1.0;
	float y = (1.0 - texCoords.y) * 2.0 - 1;

	vec4 projectedPos = vec4(x, y, z, 1.0f);

	vec4 viewSpacePos = projectedPos * inverseProjection; // simplify

	return viewSpacePos.xyz / viewSpacePos.w; 
}

void main()
{
	vec3 Normal = texture(screenSpaceTextureTwo, TexCoord).rgb;
	vec3 FragPos = CalcDepthToViewFragPos(TexCoord);

	vec3 sampleRotation = texture(theTextureNoiseSSAO, TexCoord * noiseScale);
	vec3 tangent = normalize(sampleRotation - Normal * dot(sampleRotation, Normal);
	vec3 bitangent = cross(Normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, Normal);
	
	float occlusion = 0.0;
	float radius = 0.5;
	int kernelSize = 64;

	for(int i = o; i < kernelSize; ++i)
	{
		vec3 samplePos = TBN * samples[i];
		samplePos = FragPos + samplePos * radius; // determine radius

		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset; // view to clipspace
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // 0-1 range

		float sampleDepth = CalcDepthToFragPos(offset.xy).z; // massive room for improvement - make seperate function
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	SSAO = occlusion;
}