#version 450

layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = pos;
	vec4 posZ = projection * view * vec4(pos, 1.0);

	// swizzle wont work. use stencil buffer
	gl_Position = posZ.xyww;
}