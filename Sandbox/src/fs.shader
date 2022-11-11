#version 450 core

out vec4 FragColor;

in vec2 vTexPos;
in vec4 vColor;
in float vTexIndex;

struct Material {
    sampler2D diffuse; //active texture set to 1
    sampler2D specular; //active texture set to 2
};

uniform int isQuad;

uniform sampler2D whiteTexture;     // active texture for applying colors set to 0
uniform sampler2D quadDiffuse[16];  // total 16 diffuse for batch

uniform Material material;

void main() {
    if (isQuad == 1) {
        int index = int(vTexIndex);
        FragColor = texture(quadDiffuse[index], vTexPos) * vColor;
    } else {
        FragColor = texture(material.diffuse, vTexPos);
    }

    // FragColor = vec4(1.0f);
};