#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormalPos;
layout(location = 2) in vec2 aTexPos;

out vec2 texPos;
out vec3 normalPos;
out vec3 fragPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
	texPos = aTexPos;
	normalPos = mat3(transpose(inverse(model_matrix))) * aNormalPos;
	fragPos = vec3(model_matrix * vec4(aPos, 1.0));

};