#pragma once
#include "shader.h"

#include <stb_image/stb_image.h>

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
