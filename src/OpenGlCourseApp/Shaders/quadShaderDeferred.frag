#version 430

in vec2 TexCoord;
//in float TexelSize;

out vec4 Colour;

layout (binding = 23) uniform sampler2D theTextureDepth;
layout (binding = 22) uniform sampler2D screenSpaceTexture;
layout (binding = 21) uniform sampler2D screenSpaceTextureTwo;
layout (binding = 20) uniform sampler2D screenSpaceTextureThree; 

// lights
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};
struct PointLight
{
	Light base;
	
	vec3 position;
	float constant;
	float linear;
	float exponent;
};
struct Material
{
	float specularIntensity;
	float specularPower;
};

uniform int pointLightCount;
const int MAX_POINT_LIGHTS = 3;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;

uniform float brightness;
uniform float contrast;
uniform float saturation;
uniform float gammaLevel;

uniform vec3 eyePosition;
uniform mat4 inverseProjection;
uniform mat4 inverseView;

vec3  Normal;
vec3 FragPos;

// post
vec4 ApplyGammaToneMapping(vec3 hdrColor)
{
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
	mapped = pow(mapped, vec3(1.0f / gammaLevel));

	return vec4(mapped, 1.0f);
}
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
vec4 CalcPointLightByDirection(Light light, vec3 direction)
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

	return (ambientColour) * (diffuseColour + specularColour);
}
vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 colour = CalcPointLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (colour / attenuation);
}
vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalColour += CalcPointLight(pointLights[i]);
	}
	
	return totalColour;
}
vec3 CalcFragPosFromDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;

	vec4 clipSpacePos = vec4(TexCoord * 2.0f - 1.0f, z, 1.0f);
	vec4 viewSpacePos = inverseProjection * clipSpacePos;

	viewSpacePos /= viewSpacePos.w;

	vec4 worldSpacePos = inverseView * viewSpacePos;

	return worldSpacePos.xyz;
}

void main()
{
	vec4 sampleTex2 = texture(screenSpaceTextureTwo, TexCoord);
	vec4 sampleTex3 = texture(screenSpaceTexture, TexCoord);


	// FragPos = texture(screenSpaceTextureThree, TexCoord).rgb;
	
	float depthSample = texture(theTextureDepth, TexCoord).r;
	FragPos = CalcFragPosFromDepth(depthSample);

	vec3  Albedo = sampleTex3.rgb;
	float Specular = sampleTex3.a;
	Normal = sampleTex2.rgb;
	float Height = sampleTex2.a;



	vec4 lighting = CalcPointLights();

	Colour = vec4(Albedo, 1.0f) * CalcPointLights();

		// depth works great
		// 



	//float depth = texture(theTextureDepth, TexCoord).r;
	//Colour.rgb = CalcFragPosFromDepth(depth);

	//Colour.rgb = vec3(1.0f - texture(theTextureDepth, TexCoord).r); // testing depth
	Colour = CalcMatriceBCS() * Colour;
	Colour = ApplyGammaToneMapping(Colour.rgb);
}






