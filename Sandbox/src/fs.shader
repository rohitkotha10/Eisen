#version 450 core

out vec4 FragColor;

in vec2 vTexPos;
in vec4 vColor;
in float vTexIndex;

// only 1 diffuse for each mesh
// total 16 diffuse for batch
uniform sampler2D diffuse[16];

void main() {
    int index = int(vTexIndex);
    FragColor = texture(diffuse[index], vTexPos);
    //FragColor = vec4(1.0f);
};