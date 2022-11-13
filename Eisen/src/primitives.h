#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <string>
#include <vector>

#include "shader.h"

namespace Eisen {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texPos;
        glm::vec4 color;
        float texIndex;
    };

    struct Texture {
        GLuint id;
        std::string type;
        std::string path;
        void loadTexture(std::string path, std::string typeName);
        void loadWhiteTexture();
    };
}  // namespace Eisen
