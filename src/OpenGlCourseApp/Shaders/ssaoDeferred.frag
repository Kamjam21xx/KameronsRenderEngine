#version 430

in vec2 TexCoord;

layout (location = 0) out vec3 SSAO;

layout (binding = 24) uniform sampler2D theTextureNoiseSSAO;
layout (binding = 23) uniform sampler2D theTextureDepth; // depth from eye to fragment
layout (binding = 20) uniform sampler2D screenSpaceTextureThree; // normal map

uniform mat4 projection;
uniform mat4 inverseProjection;
uniform vec3 samples[64]; // maybe bake this
uniform float ambientOcclusionRadius;
uniform float ambientOcclusionBias;

const vec2 noiseScale = vec2(1920, 1080); // 1/4 screen resolution 1080p from 4k, make it dynamic or whatevs

vec3 CalcDepthToViewFragPos(vec2 texCoords)
{
	float z = texture(theTextureDepth, texCoords).r; // * 2.0 - 1.0;

	vec4 viewSpacePos = vec4(texCoords * 2.0 - 1.0, z, 1.0) * inverseProjection;

	return viewSpacePos.xyz / viewSpacePos.w; 
}

void main()
{
	vec3 FragPos = CalcDepthToViewFragPos(TexCoord).rgb;
	vec3 Normal = normalize(texture(screenSpaceTextureThree, TexCoord).rgb);
	vec3 sampleRotation = normalize(texture(theTextureNoiseSSAO, TexCoord * noiseScale).rgb);

	// Normal.xy *= -1.0;

	vec3 tangent = normalize(sampleRotation - Normal * dot(sampleRotation, Normal));
	vec3 bitangent = cross(Normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, Normal);
	
	float occlusion = 0.0;
	float radius = ambientOcclusionRadius;
	float bias = ambientOcclusionBias;
	float kernelSize = 64.0;

	for(int i = 0; i < kernelSize; ++i)
	{
		vec3 samplePos = TBN * samples[i];
		samplePos = FragPos + samplePos * radius; // determine radius

		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset; // view to clipspace
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // 0-1 range

		float sampleDepth = CalcDepthToViewFragPos(offset.xy).z; // massive room for improvement - make seperate function
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	SSAO = vec3(occlusion);

}