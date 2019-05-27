#version 430

in vec2 TexCoord;
//in float TexelSize;

out vec4 Colour;

layout (binding = 23) uniform sampler2D theTextureDepth;
layout (binding = 22) uniform sampler2D screenSpaceTexture;
layout (binding = 21) uniform sampler2D screenSpaceTextureTwo;
layout (binding = 16) uniform sampler2D theTextureAO;
// add ssao binding

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
vec3 Albedo;
float Specular;

// post
vec4 ApplyGammaToneMapping(vec3 hdrColor)
{
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
	mapped = pow(mapped, vec3(1.0f / gammaLevel));

	return vec4(mapped, 1.0f);
}
vec4 CalcbrightnessContrastSaturation(vec4 colour)
{
    float b = brightness;
    float c = contrast;
    float s = saturation;
    
    float t = c * -.5 + .5;
    vec3 l = vec3( .3086, .6094, .0820 );
    vec3 f = (-s * l + l) * c;
    vec3 g = s * c + f;

    mat4 BCS = mat4(
        g.r, f.r, f.r, 0,
        f.g, g.g, f.g, 0,
        f.b, f.b, g.b, 0,
        vec3(b + t), 1
    );

	return vec4(BCS[0].rgg * colour.r + (BCS[1].rgr * colour.g + (BCS[2].rb * colour.b + BCS[3].x).rrg), 1.0f); 
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

	return (ambientColour) * (diffuseColour * vec4(Albedo, 1.0f) + specularColour * Specular);
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
vec3 CalcDepthToFragPos(float depth)
{
	float z = depth * 2.0f - 1.0f;

	vec4 clipSpacePos = vec4(TexCoord * 2.0f - 1.0f, z, 1.0f);
	vec4 viewSpacePos = inverseProjection * clipSpacePos;

	viewSpacePos /= viewSpacePos.w;

	vec4 worldSpacePos = inverseView * viewSpacePos;

	return worldSpacePos.xyz;
}
vec3 CalcDepthToViewFragPos(vec2 texCoords)
{
	float z = texture(theTextureDepth, texCoords).r;
	float x = texCoords.x * 2.0 - 1.0;
	float y = texCoords.y * 2.0 - 1.0;

	vec4 clipPos = vec4(x, y, z, 1.0);

	vec4 viewSpacePos = clipPos * inverseProjection; // simplify

	return viewSpacePos.xyz / viewSpacePos.w; 
}

void main()
{
	vec4 sampleTex2 = texture(screenSpaceTextureTwo, TexCoord);
	vec4 sampleTex3 = texture(screenSpaceTexture, TexCoord);
	float depthSample = texture(theTextureDepth, TexCoord).r;
	float AO = texture(theTextureAO, TexCoord).r;

	FragPos = CalcDepthToFragPos(depthSample);
	Albedo = sampleTex3.rgb;
	Specular = sampleTex3.a;
	Normal = sampleTex2.rgb;
	float Height = sampleTex2.a;

	

	vec4 lighting = CalcPointLights();

	Colour = lighting * vec4(AO, AO, AO, 1.0);

	Colour = CalcbrightnessContrastSaturation(Colour);

	Colour = ApplyGammaToneMapping(Colour.rgb);

	// study the math. math math math math math math math math math is king king king. 

	Colour.rgb = vec3(AO);



}






