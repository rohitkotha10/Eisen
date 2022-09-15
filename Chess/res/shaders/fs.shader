#version 450 core

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texPos);
};