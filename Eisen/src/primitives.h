#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image/stb_image.h>

#include <iostream>
#include <string>
#include <vector>

namespace Eisen {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texPos;

        // for my quad renderer
        glm::vec4 color;
        float texIndex;
        float radius;
        float cutoff;
        float isCircle;
    };

    struct Texture {
        GLuint id;
        std::string type;
        std::string path;
        void loadTexture(std::string path, std::string typeName);
        void loadWhiteTexture();  // for rendering colors
        void loadSkybox(std::vector<std::string> faces);
    };
}  // namespace Eisen
