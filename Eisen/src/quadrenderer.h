#pragma once
#include "primitives.h"

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
        void flush(GLuint& program);
        void drawQuadColor(GLuint& program, const Quad& quad, glm::vec4 color);  // color is set to vertices
        void drawQuadTexture(GLuint& program, const Quad& quad, Texture& texture);
        // circles drawn inside quad;
        // need percentage for radius and cutoff
        // cutoff = 0.0f for full circle
        // radius = 1.0f for diameter side
        void drawCircle(GLuint& program, const Quad& quad, float radius, float cutoff, glm::vec4 color);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;  // max textures per batch is 16 set from active 3 to 18

        int maxQuads;      // max quads in one draw call/batch
        Texture whiteTex;  // for colors
    };
}  // namespace Eisen
