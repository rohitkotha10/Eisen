#version 450 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 fragPos;
in vec2 vTexPos;
in vec4 vColor;
in float vTexIndex;

struct Model {
    sampler2D diffuse;   // active texture set to 0
    sampler2D specular;  // active texture set to 1
    vec4 color;
    int hasTexture;
};

uniform int isQuad;

uniform sampler2D quadDiffuse[16];  // total 16 diffuse for batch 2 to 17 active

uniform Model material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    if (isQuad == 1) {
        int index = int(vTexIndex);
        vec4 diffmap = texture(quadDiffuse[index], vTexPos) * vColor;
        FragColor = diffmap;
    } else {
        vec3 diffuse = vec3(0.0f);
        vec3 ambient = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        if (material.hasTexture == 0)
            diffuse = vec3(material.color);
        else {
            diffuse = vec3(texture(material.diffuse, vTexPos));
            specular = vec3(texture(material.specular, vTexPos));
        }

        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(lightPos - fragPos);
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        ambient = 0.3f * diffuse;

        float diff = max(dot(norm, lightDir), 0.0f);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);

        diffuse = diff * diffuse;
        specular = spec * specular;

        vec3 result = ambient + diffuse + specular;

        FragColor = vec4(result, 1.0);
    }
};