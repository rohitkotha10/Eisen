#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texIn;

out vec2 texPos;

uniform mat4 model_matrix;

void main()
{
	gl_Position = model_matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
	texPos = texIn;
};