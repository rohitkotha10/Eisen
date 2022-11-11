#pragma once
#include "primitives.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Eisen {

    class Mesh {
    public:
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;

        void createMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, glm::vec4 color);
        void draw(GLuint& program);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        glm::vec4 color;
        void setupMesh();
    };

    class Model {
    public:
        void loadModel(string path);
        void draw(GLuint& program);
        void shutdown();

    private:
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;

        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
        glm::vec4 loadColor(aiMaterial* mat);
    };

}  // namespace Eisen