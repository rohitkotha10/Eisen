#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexPos;
layout(location = 3) in vec4 aColor;
layout(location = 4) in float aTexIndex;
layout(location = 5) in float aIsCircle;
layout(location = 6) in float aRadius;
layout(location = 7) in float aCutoff;

out vec2 pixelPos;
out vec2 vTexPos;
out vec4 vColor;
out float vTexIndex;
out float vIsCircle;
out float vRadius;
out float vCutoff;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
    pixelPos = aTexPos;
    vTexPos = aTexPos;
    vColor = aColor;
    vTexIndex = aTexIndex;
    vIsCircle = aIsCircle;
    vRadius = aRadius;
    vCutoff = aCutoff;
};