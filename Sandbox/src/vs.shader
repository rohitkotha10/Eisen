#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texIn;

out vec2 texPos;

uniform mat4 mvp_matrix;

void main()
{
	gl_Position = mvp_matrix * vec4(aPos, 1.0);
	texPos = texIn;
};