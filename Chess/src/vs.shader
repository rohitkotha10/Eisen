#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texIn;

uniform mat4 mvp_matrix;
uniform int choice;

out vec2 texPos;
out vec2 pixelPos;

void main()
{
	if (choice == 0)
		gl_Position = mvp_matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
	else if (choice == 1)
		gl_Position = mvp_matrix * vec4(aPos.x, aPos.y, 0.1, 1.0);
	else if (choice == 2)
		gl_Position = mvp_matrix * vec4(aPos.x, aPos.y, 0.2, 1.0);
	else if (choice == 3)
		gl_Position = mvp_matrix * vec4(aPos.x, aPos.y, 0.3, 1.0);

	texPos = texIn;
	pixelPos = aPos;
};