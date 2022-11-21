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

uniform sampler2D quadDiffuse[16];  // total 16 diffuse for batch 2 to 17 active

void main() {
    if (isQuad == 1) {
        int index = int(vTexIndex);
        vec4 diffmap = texture(quadDiffuse[index], vTexPos) * vColor;

        vec2 curVec = pixelPos;
        curVec = curVec - vec2(0.5, 0.5);
        curVec *= 2;

        float distance = length(curVec);
        if (vIsCircle == 1) {  // solid small circle
            if (distance <= vRadius && distance >= vCutoff)
                FragColor = vec4(diffmap);  // select this
            else
                FragColor = vec4(0.0f);
            return;
        }

        else
            FragColor = vec4(diffmap);  // select this

    }
};