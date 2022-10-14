#version 450 core

out vec4 FragColor;

in vec2 texPos;
in vec3 normalPos;
in vec3 fragPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 viewPos;
uniform bool isFlash;


struct Material
{
	vec3 color;
	float shininess;
	float ambientStrength;

};

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;

	float cutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform Material material;

void main()
{
	vec3 norm = normalize(normalPos);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	ambient = material.ambientStrength * vec3(texture(tex0, texPos));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	float diff = max(dot(norm, lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	diffuse = diff * light.color * vec3(texture(tex0, texPos));
	specular = spec * light.color * vec3(texture(tex1, texPos));

	diffuse *= intensity;
	specular *= intensity;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	if (isFlash == false) 
	{
		diffuse = glm::vec3(0.0f);
		specular = glm::vec3(0.0f);
	}

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
};