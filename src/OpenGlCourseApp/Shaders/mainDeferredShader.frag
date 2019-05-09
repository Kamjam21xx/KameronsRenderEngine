#version 430

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;
in mat3 tTBN;

layout (location = 0) out vec3 Position;  // re-calculate in next shader instead of passing a whole texture
layout (location = 1) out vec4 NormalHeight;
layout (location = 2) out vec4 ColourSpecular;

// variables
float heightScale;
uniform float heightPOM;
uniform vec3 eyePosition;

// textures
layout (binding = 1) uniform sampler2D theTextureDiffuse; // alpha = specular;
layout (binding = 5) uniform sampler2D theTextureNormal; // alpha = height;


vec2 ParallaxMapping(vec2 TexCoords, vec3 eyeDir)
{
	eyeDir.y *= -1.0f;

	const float minLayers = 32.0f;
	const float maxLayers = 64.0f;
	float layerCount = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), eyeDir)));

	float layerDepth = 1.0f / layerCount;
	float currentLayerDepth = 0.0f; 
	
	vec2 P = eyeDir.xy / eyeDir.z * heightScale;
	vec2 deltaTexCoords = P / layerCount;

	vec2 currentTexCoords = TexCoords;
	float currentDepthMapValue = 1.0f - float(texture(theTextureNormal, currentTexCoords).a);

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords += deltaTexCoords;
		currentDepthMapValue = 1.0f - float(texture(theTextureNormal, currentTexCoords).a);
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0f - float(texture(theTextureNormal, prevTexCoords).a) - currentLayerDepth + layerDepth;
	
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return currentTexCoords; 
}


void main()
{
	// Parallax_occlusion_mapping
	heightScale = 0.0175f * heightPOM; // height not working for an unkown reason. will fix later
	vec3 TangentViewPos = tTBN * eyePosition;
	vec3 TangentFragPos = tTBN * FragPos;
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 TexCoords = ParallaxMapping(TexCoord, viewDir);


	// Normal_mapping
	vec4 NormalTexture = texture(theTextureNormal, TexCoords) * 2.0f - 1.0f;
	// NormalTexture.rgb *=
	NormalTexture.xy *= -1.0f;
	NormalTexture.rgb = normalize(TBN * NormalTexture.rgb);


	// outputs
	Position = FragPos;
	NormalHeight = NormalTexture;
	ColourSpecular = texture(theTextureDiffuse, TexCoords);
}