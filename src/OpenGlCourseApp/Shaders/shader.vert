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

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

vec4 CalcVertexWorldPos(mat4 P, mat4 V, mat4 M, vec3 vp)
{
	// P projection | V view | M model | vp vertex position

	// projection * view
	mat4 PV = mat4(
		P[0].x * V[0].x,
		P[1].y * V[0].y,
		P[2].z * V[0].z,
		P[2].w * V[0].z,
		P[0].x * V[1].x,
		P[1].y * V[1].y,
		P[2].z * V[1].z,
		P[2].w * V[1].z,
		P[0].x * V[2].x,
		P[1].y * V[2].y,
		P[2].z * V[2].z + P[3].z * V[2].w,
		P[2].w * V[2].z,
		P[0].x * V[3].x,
		P[1].y * V[3].y,
		P[2].z * V[3].z + P[3].z, // * V[3].w, // 3,3 = 1 ?
		P[2].w * V[3].z
	);

	return PV * M * vec4(vp, 1.0); 
}

void main()
{
	//gl_Position = projection * view * model * vec4(pos, 1.0f);
	gl_Position = CalcVertexWorldPos(projection, view, model, pos);
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
	TexCoord = tex;
	FragPos = (model * vec4(pos, 1.0f)).xyz;

	vec3 T = normalize( vec3(model * vec4(tang, 0.0f)   )   );
	vec3 N = normalize( vec3(model * vec4(norm, 0.0f)   )   );

	TBN = mat3(T, cross( T , N ), N);
}