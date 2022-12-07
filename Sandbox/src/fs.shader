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
    int hasTexture;
};

uniform Model material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 diffuse = vec3(0.0f);
    vec3 ambient = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    if (material.hasTexture == 0) {
        diffuse = vec3(material.color);
        specular = 0.3 * vec3(1.0f);
    } else {
        diffuse = vec3(texture(material.diffuse, vTexPos));
        specular = 0.3 * vec3(1.0f);
    }

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 8.0f);

    ambient = 0.03f * diffuse;
    diffuse = diff * diffuse;
    specular = spec * specular;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
};