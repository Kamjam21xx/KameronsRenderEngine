#version 430

layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

vec4 CalcPos(mat4 P, mat4 V, vec3 vp)
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


	return PV * vec4(vp, 1.0);
}

void main()
{
	TexCoords = pos;
	vec4 posZ = CalcPos(projection, view, pos);

	// swizzle wont work. use stencil buffer
	gl_Position = posZ.xyww;
}