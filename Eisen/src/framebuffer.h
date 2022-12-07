#pragma once
#include "primitives.h"

namespace Eisen {
    class Framebuffer {
    public:
        void create(int scrw, int scrh);
        void createDepthFBO(int scrw, int scrh);
        void shutdown();
        void drawfbo(Program& program); //draw the texture on a quad

        GLuint id;
        GLuint texture;
        GLuint rbo;

        GLuint quadvao;
        GLuint quadvbo;
        GLuint quadebo;
    };
}  // namespace Eisen
