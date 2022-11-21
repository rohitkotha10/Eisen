#version 450 core

out vec4 FragColor;

in vec2 pixelPos;
in vec2 vTexPos;
in vec4 vColor;
in float vIsCircle;
in float vTexIndex;
in float vRadius;
in float vCutoff;

uniform int isQuad;

uniform sampler2D quadDiffuse[16];  // total 16 diffuse max for batch

void main() {
    int index = int(vTexIndex);
    vec4 diffmap = texture(quadDiffuse[index], vTexPos) * vColor;

    if (vIsCircle == 1) {
        // shift lower left to center
        vec2 curVec = pixelPos;
        curVec = curVec - vec2(0.5, 0.5);
        curVec *= 2;
        float distance = length(curVec);
        if (distance <= vRadius && distance >= vCutoff)
            FragColor = vec4(diffmap);
        else
            FragColor = vec4(0.0f);
        return;
    } else {
        FragColor = vec4(diffmap);
    }
};