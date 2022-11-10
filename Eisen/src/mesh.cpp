#include "mesh.h"

namespace Eisen {

    Texture loadMeshTexture(string path, string typeName) {
        Texture texture;

        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = 3;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);

        texture.type = typeName;
        texture.path = path;
        return texture;
    }

    void Mesh::createMesh(vector<Vertex>& vertices, vector<GLuint>& indices, Texture& texture) {
        this->vertices = vertices;
        this->indices = indices;
        this->texture = texture;
    }

    void Renderer::init() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        Texture myTex = loadMeshTexture("../media/wood.jpg", "diffuse");
        float texFloat = (float)myTex.id;
        vector<Vertex> myVertices;
        Vertex a0;
        a0.position = glm::vec3(-0.5f, -0.5f, 0.0f);
        a0.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a0.texPos = glm::vec2(0.0f, 0.0f);
        a0.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a0.texIndex = texFloat;

        Vertex a1;
        a1.position = glm::vec3(-0.5f, 0.5f, 0.0f);
        a1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a1.texPos = glm::vec2(0.0f, 1.0f);
        a1.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a1.texIndex = texFloat;

        Vertex a2;
        a2.position = glm::vec3(0.5f, 0.5f, 0.0f);
        a2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a2.texPos = glm::vec2(1.0f, 1.0f);
        a2.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a2.texIndex = texFloat;

        Vertex a3;
        a3.position = glm::vec3(0.5f, -0.5f, 0.0f);
        a3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a3.texPos = glm::vec2(1.0f, 0.0f);
        a3.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        a3.texIndex = texFloat;

        myVertices.push_back(a0);
        myVertices.push_back(a1);
        myVertices.push_back(a2);
        myVertices.push_back(a3);

        vector<GLuint> myIndices = {0, 1, 2, 2, 3, 0};

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(Vertex), &myVertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texPos));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));
        glEnableVertexAttribArray(4);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 10000 * sizeof(GLuint), &myIndices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer::shutdown() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i].id);
    }

    void Renderer::beginBatch() {
        vertices.clear();
        indices.clear();
        textures.clear();
    }

    void Renderer::draw(GLuint& program, Mesh& mesh) {
        // copy vertices to dynamic array and update indices
        // 1 diffuse and 1 specular for each triangle, 8, 8 for each batch

        if (indices.size() >= 30000 || textures.size() >= 16) {
            endBatch();
            flush(program);
            beginBatch();
        }
        int curInd = vertices.size();
        for (int i = 0; i < mesh.vertices.size(); i++) vertices.push_back(mesh.vertices[i]);
        for (int i = 0; i < mesh.indices.size(); i++) indices.push_back(mesh.indices[i] + curInd);

        int check = 0;

        for (int j = 0; j < textures.size(); j++) {
            if (mesh.texture.id == textures[j].id) {
                check = 1;
                break;
            }
        }

        if (check != 1) textures.push_back(mesh.texture);
    }

    void Renderer::endBatch() {
       /* glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);*/
    }

    void Renderer::flush(GLuint& program) {
        glUseProgram(program);
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textures[i].id);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}  // namespace Eisen
