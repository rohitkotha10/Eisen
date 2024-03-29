#pragma once

#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace Eisen {
    class Quad {
    public:
        Quad(float x, float y, float size);  // lower left corner x,y. square side(size)
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Texture tex;
    };

    class QuadRenderer {
    public:
        void init(int maxQuads);
        void beginBatch();
        void endBatch();
        void flush(Program& program);
        void drawQuadColor(Program& program, const Quad& quad, glm::vec4 color);  // color is set to vertices
        void drawQuadTexture(Program& program, const Quad& quad, Texture& texture);

        // circles drawn inside quad;
        // need percentage for radius and cutoff
        // cutoff = 0.0f for full circle
        // radius = 1.0f for diameter side
        void drawCircle(Program& program, const Quad& quad, float radius, float cutoff, glm::vec4 color);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;  // max textures per batch is 16

        int maxQuads;                   // max quads in one draw call/batch
        Texture whiteTex;               // for colors
    };
}  // namespace Eisen
