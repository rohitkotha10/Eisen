#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texIn;

uniform mat4 trans_matrix;

out vec2 texPos;

void main()
{
	gl_Position = trans_matrix *  vec4(aPos.x, aPos.y, 0.0, 1.0);
	texPos = texIn;
};