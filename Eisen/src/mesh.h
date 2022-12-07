#pragma once
#include "primitives.h"

namespace Eisen {
    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        glm::vec4 color;

        void createMesh(
            std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, glm::vec4 color);
        void draw(Program& program);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };
}  // namespace Eisen
