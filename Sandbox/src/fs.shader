#version 450 core
// Blinn Phong
out vec4 FragColor;

in vec3 fragPos;
in vec3 vNormal;
in vec2 vTexPos;

struct Model {
    sampler2D diffuse;   // active texture set to 0
    sampler2D specular;  // active texture set to 1
    vec4 color;
    int hasDiffMap;
    int hasSpecMap;
};

uniform Model material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 diffuse = vec3(0.0f);
    vec3 ambient = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    if (material.hasDiffMap == 0)
        diffuse = vec3(material.color);
    else
        diffuse = vec3(texture(material.diffuse, vTexPos));

    if (material.hasSpecMap == 0)
        specular = 0.3 * vec3(1.0f);
    else
        specular = vec3(texture(material.specular, vTexPos));

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 8.0f);

    ambient = 0.03f * diffuse;
    diffuse = diff * diffuse;
    specular = spec * specular;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
};