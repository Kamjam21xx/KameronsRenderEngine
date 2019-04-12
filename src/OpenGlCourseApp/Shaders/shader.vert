#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tang;
layout (location = 4) in vec3 bitang;

out vec2 TexCoord;
out vec3 FragPos; 
out vec4 DirectionalLightSpacePos;
out mat3 TBN;
out mat3 tTBN;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
	TexCoord = tex;
	FragPos = (model * vec4(pos, 1.0f)).xyz;

	 vec3 T = normalize( vec3(model * vec4(tang, 0.0f)   )   );
	 vec3 N = normalize( vec3(model * vec4(norm, 0.0f)   )   );

	 TBN = mat3(T, cross( T , N ), N);
	 tTBN = transpose(TBN);
}