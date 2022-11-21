#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>

namespace Eisen {
    std::string parse(std::string shaderPath);

    class Program {
    public:
        void create(std::string vsSource, std::string fsSource);
        void use();
        void shutdown();
        void setInt(std::string name, int value);
        void setFloat(std::string name, float value);
        void setMat4(std::string name, const glm::mat4& data);
        void setVec4(std::string name, float x, float y, float z, float a);
        void setVec4(std::string name, const glm::vec4& data);
        void setVec3(std::string name, float x, float y, float z);
        void setVec3(std::string name, const glm::vec3& data);
        void setIntArray(std::string name, int size, int* data);

    private:
        GLuint id;
    };
}  // namespace Eisen
