#version 450 core

out vec4 FragColor;

in vec2 vTexPos;

uniform sampler2D tex;

void main() {
    FragColor = vec4(vec3(1.0f - texture(tex, vTexPos)), 1.0f);
    float avg = (FragColor.x + FragColor.y + FragColor.z) / 3.0f;
    FragColor = vec4(avg, avg, avg, 1.0f);
}