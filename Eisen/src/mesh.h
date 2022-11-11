#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <string>
#include <vector>

#include "shader.h"

using namespace std;

namespace Eisen {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texPos;
        glm::vec4 color;
        float texIndex;
    };

    struct Texture {
        GLuint id;
        string type;
        string path;
    };

    Texture loadMeshTexture(string path, string typeName);

    class Mesh {
    public:
        vector<Vertex> vertices;
        vector<GLuint> indices;
        Texture texture;

        void createMesh(vector<Vertex>& vertices, vector<GLuint>& indices, Texture& texture);
    };

    

}  // namespace Eisen
