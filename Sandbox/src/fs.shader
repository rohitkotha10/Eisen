#version 450 core

out vec4 FragColor;

in vec2 texPos;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
};

uniform Material material;

void main()
{
	FragColor = texture(material.diffuse1, texPos);
};