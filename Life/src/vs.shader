#version 450 core

layout(location = 0) in vec2 aPos;
uniform mat4 mvp_matrix;

void main()
{
	gl_Position = mvp_matrix * vec4(aPos.x, aPos.y, 0.1f, 1.0f);
};