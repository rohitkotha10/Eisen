#version 450 core

layout(location = 0) in vec3 aPos;
uniform mat4 trans_matrix;

void main()
{
	gl_Position = trans_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
};