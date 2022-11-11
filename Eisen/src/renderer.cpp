#include "renderer.h"

namespace Eisen {

    void Renderer::init() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

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

    void Renderer::drawMesh(GLuint& program, Mesh& mesh) {
        // copy vertices to dynamic array and update indices
        // 1 diffuse and 1 specular for each triangle, 8, 8 for each batch

        if (indices.size() >= 100000 || textures.size() >= 16) {
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

    void Renderer::drawModel(GLuint& program, Model& model) {
        for (int i = 0; i < model.meshes.size(); i++) {
            beginBatch();
            drawMesh(program, model.meshes[i]);
            endBatch();
            flush(program);
        }
    }

    void Renderer::endBatch() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    void Renderer::flush(GLuint& program) {
        glUseProgram(program);
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textures[i].id);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}  // namespace Eisen
