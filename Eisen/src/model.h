#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

using namespace std;

namespace Eisen {
    class Model {
    public:
        void loadModel(string path);
        vector<Mesh> meshes;

    private:
        string directory;
        vector<Texture> textures_loaded;

        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };

}  // namespace Eisen
