#pragma once
#include "primitives.h"

using namespace std;

namespace Eisen {

    class Quad {
    public:
        Quad(float x, float y, float size); //lower left corner x,y. square side(size)
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        Texture tex;
    };
    class QuadRenderer {
    public:
        void init(int maxQuads);
        void beginBatch();
        void endBatch();
        void flush(GLuint& program);
        void drawQuadColor(GLuint& program, const Quad& quad, glm::vec4 color); //color is set to vertices
        void drawQuadTexture(GLuint& program, const Quad& quad, Texture& texture);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures; //max textures per batch is 16 set from active 3 to 18

        int maxQuads; //max quads in one draw call/batch
        Texture whiteTex; //for colors
    };
}  // namespace Eisen
