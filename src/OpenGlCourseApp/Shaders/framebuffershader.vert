#version 430

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tex;

out vec2 TexCoords;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	TexCoords = tex;
}