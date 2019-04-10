#version 450

in vec2 TexCoord;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;
in mat3 TBN;
in mat3 tTBN;

out vec4 colour;


// variables
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
float heightScale;

vec3 Normal;

// structs
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float specularPower;
};


// uniforms
uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;
uniform vec3 eyeDirection;

// textures
layout (binding = 1) uniform sampler2D theTextureDiffuse;
layout (binding = 4) uniform sampler2D theTextureSpecular;
layout (binding = 5) uniform sampler2D theTextureNormal;
layout (binding = 14) uniform sampler2D theTextureHeight;
layout (binding = 2) uniform sampler2D directionalShadowMap;
layout (binding = 6) uniform samplerCube skyBoxTexture;

vec4 diffuse;
vec4 specular;

// other
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


// functions
vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.specularPower);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour * diffuse) + (1.0 - shadowFactor) * (diffuseColour * diffuse + specularColour * specular);
}

vec4 CalcPointLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(Normal, direction), 0.0f);
	vec4 diffuseColour = vec4(light.colour * (light.diffuseIntensity * diffuseFactor + diffuseFactor * 0.2f), 1.0f);
	
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.specularPower);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour * diffuse) + (1.0 - shadowFactor) * (diffuseColour * diffuse + specularColour * specular);
}

float CalcPointShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPos - light.position;
	float currentDepth = length(fragToLight);
	
	float shadow = 0.0;
	float bias   = 0.015;
	int samples  = 40;
	float viewDistance = length(eyePosition - FragPos);
	float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 85.0;
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= omniShadowMaps[shadowIndex].farPlane;
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);  
	
	return shadow;
}

float CalcShadowFactor(vec4 DirectionalLightSpacePos)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction); // lightDirection
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}
	
	return shadow;
}

vec4 CalcDirectionalLight(vec4 DirectionalLightSpacePos)
{
	float ShadowFactor = CalcShadowFactor(DirectionalLightSpacePos);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, ShadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
	vec3 direction = FragPos - pLight.position; // TBN * (eyeDirection + pLight.position)// vec3(); // lightDirection
	float distance = length(direction);
	direction = normalize(direction);
	
	float shadowFactor = CalcPointShadowFactor(pLight, shadowIndex);
	
	vec4 colour = CalcPointLightByDirection(pLight.base, direction, shadowFactor);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position); // lightDirection
	float slFactor = dot(rayDirection, sLight.direction);
	
	if(slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base, shadowIndex);
		
		// return colour * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		return colour * ((sLight.edge - slFactor) / (sLight.edge - 1.0f));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalColour += CalcPointLight(pointLights[i], i);
	}
	
	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{		
		totalColour += CalcSpotLight(spotLights[i], i + pointLightCount);
	}
	
	return totalColour;
}

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
	float currentDepthMapValue = 1.0f - float(texture(theTextureHeight, currentTexCoords).r);

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords += deltaTexCoords;
		currentDepthMapValue = 1.0f - float(texture(theTextureHeight, currentTexCoords).r);
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0f - float(texture(theTextureHeight, prevTexCoords).r) - currentLayerDepth + layerDepth;
	
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	 return currentTexCoords; 
}

void main()
{
	// Tangent Bitangent Normal
	vec3 TangentViewPos = tTBN * eyePosition;
	vec3 TangentFragPos = tTBN * FragPos;

	// Parallax occlusion mapping
	heightScale = 0.0175f;
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 TexCoord = ParallaxMapping(TexCoord, viewDir);

	// Normal mapping
	vec3 NormalTexture = texture(theTextureNormal, TexCoord).rgb * 2.0f - 1.0f;
	NormalTexture.xy *= -1.0f;
	Normal = normalize(TBN * NormalTexture);
    
	// Texture sampling
	diffuse = texture(theTextureDiffuse, TexCoord);
	specular = texture(theTextureSpecular, TexCoord);
	// float alpha = texture(theTextureDiffuse, TexCoord).a;

	// Light calculation
	vec4 shadowFactor = CalcPointLights();
	//shadowFactor = CalcDirectionalLight(DirectionalLightSpacePos);
	//shadowFactor += CalcSpotLights();

	// Reflection
	vec3 I = normalize(FragPos - eyePosition);
	vec3 R = reflect(I, normalize(Normal));
	vec4 reflectColor = texture(skyBoxTexture, R);

	// Channel mixing
	colour = shadowFactor * diffuse;
	//shadowFactor = AmbientColourG + shadowFactor * (DiffuseColourG + SpecularColourG);
	//colour = (AmbientColourG * diffuse) + shadowFactor * (DiffuseColourG * diffuse + SpecularColourG * reflectColor * specular);


	// Gamma
	float gamma = 2.3;
	colour.rgb = pow(colour.rgb, vec3(1.0/gamma));	

	//colour = DiffuseColourG;
	//colour = shadowFactor;
	//colour.rgb = vec3(pointLights[0].base.colour);
	//colour = SpecularColourG;
	//colour = AmbientColourG;
	//colour = shadowFactor;

}