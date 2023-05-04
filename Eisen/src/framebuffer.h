#pragma once

#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

namespace Eisen {
    class Framebuffer {
    public:
        void create(int scrw, int scrh);
        void createDepthFBO(int scrw, int scrh);
        void shutdown();
        void drawfbo(Program& program);  // draw the texture on a quad

        GLuint id;
        GLuint texture;
        GLuint rbo;

        GLuint quadvao;
        GLuint quadvbo;
        GLuint quadebo;
    };
}  // namespace Eisen
