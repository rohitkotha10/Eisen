#include "mesh.h"

using namespace std;

namespace Eisen {
    void Mesh::createMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, glm::vec4 color) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->color = color;

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

        glBindVertexArray(0);
    }

    void Mesh::draw(Program& program) {
        int numDiffuse = 0;
        int numSpecular = 0;
        for (unsigned int i = 0; i < textures.size(); i++) {
            // allowing only 1 diffuse and 1 specular map for model

            string name = textures[i].type;
            if (name == "diffuse" && numDiffuse <= 1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                program.setInt("material.diffuse", 0);
                numDiffuse++;
            } else if (name == "specular" && numSpecular <= 1) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                program.setInt("material.specular", 1);
                numSpecular++;
            } else {
                cout << "Unknown texture type: " << textures[i].path << endl;
            }
        }
        if (numSpecular > 1 || numDiffuse > 1) cout << "Only 1 diffuse and 1 specular supported!" << endl;

        if (numDiffuse == 0)
            program.setInt("material.hasTexture", 0);
        else
            program.setInt("material.hasTexture", 1);

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(vao);
        program.setVec4("material.color", this->color);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::shutdown() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i].id);
    }
}  // namespace Eisen
