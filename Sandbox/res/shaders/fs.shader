#version 450 core

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	FragColor = mix(texture(tex0, texPos), texture(tex1, texPos), 0.8);
};