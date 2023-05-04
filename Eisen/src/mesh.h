#pragma once

#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

namespace Eisen {
    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        glm::vec4 color;

        // pass empty texture vector if you want to use a color
        void createMesh(
            std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, glm::vec4 color);
        void createPlaneTexture(glm::vec3 center, float side, Texture texture, float texScale);
        void createPlaneColor(glm::vec3 center, float side, glm::vec4 color);
        void draw(Program& program);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };
}  // namespace Eisen
