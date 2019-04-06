#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tang;
layout (location = 4) in vec3 bitang;

out vec2 TexCoord;
out vec3 FragPos; 
out vec4 DirectionalLightSpacePos;
out vec3 T;
out vec3 N;
out vec3 viewDirection;
//out vec3 normal;
//out vec3 Tangent;
//out vec3 B;
//out mat3 TBN;
//out vec3 camVertPos;

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

	//mat4 view = view * model;
	mat4 mvMatrix = view * model;
	mat3 mv3x3 = mat3(mvMatrix);
	vec3 viewDirection = vec3(view[3].xyz) * mv3x3;
	//vec3 viewDirection = vec3(mvMatrix[3].xyz);

	 T = normalize( vec3(model * vec4(tang, 0.0f)   )   );
	 N = normalize( vec3(model * vec4(norm, 0.0f)   )   );
	// B = normalize( cross( N , T ) );

	// gl_ModelViewMatrix * gl_Vertex

	// TBN = transpose(mat3(T, B, N));
}