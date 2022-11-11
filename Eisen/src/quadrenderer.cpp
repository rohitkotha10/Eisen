#include "quadrenderer.h"

namespace Eisen {

    Quad::Quad(float x, float y, float size) {
        Vertex a0;
        a0.position = glm::vec3(x, y, 0.0f);
        a0.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a0.texPos = glm::vec2(0.0f, 0.0f);

        Vertex a1;
        a1.position = glm::vec3(x, y + size, 0.0f);
        a1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a1.texPos = glm::vec2(0.0f, 1.0f);

        Vertex a2;
        a2.position = glm::vec3(x + size, y + size, 0.0f);
        a2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a2.texPos = glm::vec2(1.0f, 1.0f);

        Vertex a3;
        a3.position = glm::vec3(x + size, y, 0.0f);
        a3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        a3.texPos = glm::vec2(1.0f, 0.0f);

        vertices = {a0, a1, a2, a3};
        indices = {0, 1, 2, 2, 3, 0};
    }

    void QuadRenderer::init(int maxQuads) {
        this->maxQuads = maxQuads;
        this->whiteTex.loadWhiteTexture();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, maxQuads * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

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

    void QuadRenderer::shutdown() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i].id);
    }

    void QuadRenderer::beginBatch() {
        vertices.clear();
        indices.clear();
        textures.clear();
    }

    void QuadRenderer::drawQuadColor(GLuint& program, const Quad& quad, glm::vec4 color) {
        // copy vertices to dynamic array and update indices
        if (indices.size() >= maxQuads * 6 || textures.size() >= 16) {
            endBatch();
            flush(program);
            beginBatch();
        }

        unsigned int curInd = vertices.size();
        vector<unsigned int> temp = {curInd + 0, curInd + 1, curInd + 2, curInd + 2, curInd + 3, curInd + 0};
        for (unsigned int i: temp) indices.push_back(i);

        for (Vertex vert: quad.vertices) {
            vert.color = color;
            vert.texIndex = 0.0f;
            vertices.push_back(vert);
        }
    }

    void QuadRenderer::drawQuadTexture(GLuint& program, const Quad& quad, Texture& texture) {
        // copy vertices to dynamic array and update indices
        if (indices.size() >= maxQuads * 6 || textures.size() >= 16) {
            endBatch();
            flush(program);
            beginBatch();
        }

        float indTex;

        int check = 0;
        for (int i = 0; i < textures.size(); i++) {
            if (texture.path == textures[i].path) {
                check = 1;
                indTex = (float)i + 1;
                break;
            }
        }

        if (check != 1) {
            textures.push_back(texture);
            indTex = float(textures.size());
        }

        unsigned int curInd = vertices.size();
        vector<unsigned int> temp = {curInd + 0, curInd + 1, curInd + 2, curInd + 2, curInd + 3, curInd + 0};
        for (unsigned int i: temp) indices.push_back(i);

        for (Vertex vert: quad.vertices) {
            vert.color = glm::vec4(1.0f);
            vert.texIndex = indTex;
            vertices.push_back(vert);
        }
    }

    void QuadRenderer::endBatch() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    void QuadRenderer::flush(GLuint& program) {
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, whiteTex.id);

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE3 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        int data[16];
        for (int i = 0; i < 16; i++) data[i] = i + 2;
        setIntArray(program, "quadDiffuse", 16, &data[0]);

        glBindVertexArray(vao);
        setInt(program, "isQuad", 1);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}  // namespace Eisen
