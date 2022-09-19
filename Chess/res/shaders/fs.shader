#version 450 core

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D tex0;
uniform vec3 color;

void main()
{
	FragColor = texture(tex0, texPos) * vec4(color, 1.0);
};