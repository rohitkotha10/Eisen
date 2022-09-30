#version 450 core

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D tex0;
uniform vec3 color;
uniform int choice;

void main()
{
	if (choice == 1)
		FragColor = texture(tex0, texPos);
	else
		FragColor = vec4(color, 0.5);

};