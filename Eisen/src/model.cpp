#include "model.h"

namespace Eisen {

    void Mesh::createMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, glm::vec4 color) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->color = color;

        setupMesh();
    }

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texPos));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void Mesh::draw(GLuint& program) {
        int numDiffuse = 0;
        int numSpecular = 0;
        for (unsigned int i = 0; i < textures.size(); i++) {
            // allowing only 1 diffuse and 1 specular map for model

            string name = textures[i].type;
            if (name == "diffuse" && numDiffuse <= 1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                setInt(program, "material.diffuse", 0);
                numDiffuse++;
            } else if (name == "specular" && numSpecular <= 1) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                setInt(program, "material.specular", 1);
                numSpecular++;
            } else {
                cout << "Unknown texture type: " << textures[i].path << endl;
            }
        }
        if (numSpecular > 1 || numDiffuse > 1) cout << "Only 1 diffuse and 1 specular supported!" << endl;

        if (numDiffuse == 0)
            setInt(program, "material.hasTexture", 0);
        else
            setInt(program, "material.hasTexture", 1);

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(vao);
        setVec4(program, "material.color", this->color);
        setInt(program, "isQuad", 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::shutdown() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i].id);
    }

    void Model::draw(GLuint& program) {
        for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].draw(program);
    }

    void Model::loadModel(string path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) { processNode(node->mChildren[i], scene); }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        glm::vec4 color;

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
            vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
            color = loadColor(material);

            for (int i = 0; i < diffuseMaps.size(); i++) textures.push_back(diffuseMaps[i]);

            for (int i = 0; i < specularMaps.size(); i++) textures.push_back(specularMaps[i]);
        }

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            vertex.color = color;

            if (mesh->mTextureCoords[0])  // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texPos = vec;
            } else
                vertex.texPos = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
        }

        Mesh temp;
        temp.createMesh(vertices, indices, textures, color);
        return temp;
    }

    void Model::shutdown() {
        for (int i = 0; i < meshes.size(); i++) { meshes[i].shutdown(); }
    }

    vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            string filename = string(str.C_Str());
            filename = directory + '/' + filename;

            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (textures_loaded[j].path == filename) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {  // if texture hasn't been loaded already, load it
                Texture texture;
                texture.loadTexture(filename, typeName);
                textures_loaded.push_back(texture);  // add to loaded textures
                textures.push_back(texture);
            }
        }
        return textures;
    }

    glm::vec4 Model::loadColor(aiMaterial* mat) {
        aiColor3D color(0.f, 0.f, 0.f);
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        return glm::vec4(color.r, color.g, color.b, 1.0f);
    }
}  // namespace Eisen
