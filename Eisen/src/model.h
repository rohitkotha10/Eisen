#pragma once
#include "primitives.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Eisen {

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        void createMesh(
            std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, glm::vec4 color);
        void draw(Program& program);
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
        void loadModel(std::string path);
        void draw(Program& program);
        void shutdown();

    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;

        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        glm::vec4 loadColor(aiMaterial* mat);
    };

}  // namespace Eisen
