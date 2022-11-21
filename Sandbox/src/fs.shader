#version 450 core
// Phong Lighting shader
out vec4 FragColor;

in vec3 vNormal;
in vec3 fragPos;
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

uniform samplerCube skybox;

void main() {
    vec3 incident = normalize(fragPos - viewPos);
    vec3 reflect = reflect(incident, normalize(vNormal));

    float ratio = 1.00 / 1.52;
    vec3 refract = refract(incident, normalize(vNormal), ratio);
    FragColor = vec4(texture(skybox, refract).rgb, 1.0f);
};