#pragma once
#include "shader.h"

namespace Eisen {
    class Framebuffer {
    public:
        void create(int scrw, int scrh);
        void shutdown();

        GLuint id;
        GLuint texture;
        GLuint rbo;
    };
}  // namespace Eisen
