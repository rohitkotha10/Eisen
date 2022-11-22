#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 vTexPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    vec4 pos = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    vTexPos = aPos;
}