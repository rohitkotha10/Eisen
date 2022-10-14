#version 450 core

out vec4 FragColor;

in vec2 texPos;
in vec3 normalPos;
in vec3 fragPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 viewPos;


struct Material
{
	vec3 color;
	float shininess;
	float ambientStrength;

};

struct Light
{
	vec3 position;
	vec3 color;
};

uniform Light light;
uniform Material material;

void main()
{
	vec3 norm = normalize(normalPos);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float diff = max(dot(norm, lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = material.ambientStrength * vec3(texture(tex0, texPos));
	vec3 diffuse = diff * light.color * vec3(texture(tex0, texPos));
	vec3 specular = spec * light.color * vec3(texture(tex1, texPos));
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
};