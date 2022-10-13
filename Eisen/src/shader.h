#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>

namespace Eisen
{
	std::string parse(std::string shaderPath);
	void setInt(GLuint program, std::string name, int value);
	void setFloat(GLuint program, std::string name, float value);
	void setBool(GLuint program, std::string name, bool value);
	void setMat4(GLuint program, std::string name, const glm::mat4& data);
	void setVec4(GLuint program, std::string name, float x, float y, float z, float a);
	void setVec4(GLuint program, std::string name, const glm::vec4& data);
	void setVec3(GLuint program, std::string name, float x, float y, float z);
	void setVec3(GLuint program, std::string name, const glm::vec3& data);
	void setTexture(GLuint program, std::string name, int destination);
}

