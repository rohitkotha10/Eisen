#version 450 core

out vec4 FragColor;

in vec2 texPos;

uniform int tex;
uniform sampler2D tex0;
uniform vec3 color;

void main()
{
	if (tex == 1)
		FragColor = texture(tex0, texPos);
	else
		FragColor = vec4(color.x, color.y, color.z, 1.0f);
};