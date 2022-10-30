#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "shader.h"

using namespace std;

namespace Eisen {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texPos;
    };

    struct Texture {
        GLuint id;
        string type;
        string path;
    };

    class Mesh {
    public:
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
        void draw(GLuint& program);

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        void setupMesh();
    };

}  // namespace Eisen
