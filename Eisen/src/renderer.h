#pragma once
#include "mesh.h"
#include "model.h"

using namespace std;

namespace Eisen {
    class Renderer {
    public:
        void init();
        void beginBatch();
        void endBatch();
        void flush(GLuint& program);
        void drawMesh(GLuint& program, Mesh& mesh);
        void drawModel(GLuint& program, Model& model);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
    };
}  // namespace Eisen
