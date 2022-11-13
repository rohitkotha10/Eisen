#version 450 core

out vec4 FragColor;

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

void main() {
    if (isQuad == 1) {
        int index = int(vTexIndex);
        FragColor = texture(quadDiffuse[index], vTexPos) * vColor;
    } else {
        if (material.hasTexture == 0)
            FragColor = material.color;  // diffuse set to whiteTex if no map
        else
            FragColor = texture(material.diffuse, vTexPos);
    }
};