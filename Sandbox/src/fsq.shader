#version 450 core

out vec4 FragColor;

in vec2 vTexPos;

uniform sampler2D myTex;

void main() {
    vec4 result = texture(myTex, vTexPos);
    FragColor = vec4(vec3(1.0f) - result.rgb, 1.0f);
};