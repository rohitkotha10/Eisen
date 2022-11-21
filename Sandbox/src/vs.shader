#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexPos;

out vec3 vNormal;
out vec3 fragPos;
out vec2 vTexPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
    vTexPos = aTexPos;
    vNormal = mat3(transpose(inverse(model_matrix))) * aNormal;
    fragPos = vec3(model_matrix * vec4(aPos, 1.0));
};