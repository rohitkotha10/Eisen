#include "framebuffer.h"

using namespace std;

namespace Eisen {
    void Framebuffer::create(int scrw, int scrh) {
        glGenFramebuffers(1, &this->id);
        glBindFramebuffer(GL_FRAMEBUFFER, this->id);

        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scrw, scrh);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scrw, scrh, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Error: Incomplete framebuffer" << endl;
        else
            cout << "GOOD" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::shutdown() {
        glDeleteFramebuffers(1, &this->id);
        glDeleteRenderbuffers(1, &this->rbo);
        glDeleteTextures(1, &this->texture);
    }
}  // namespace Eisen
