#pragma once

#include "texture/Texture2D.hpp"
#include <cstdio>

struct FBO {
  GLuint id = 0;
  GLsizei size = 0;

  FBO() = default;

  FBO(GLsizei size) : size(size) {
    glGenFramebuffers(size, &id);
  }

  void operator=(FBO rhs) {
    clear();
    id = rhs.id;
    size = rhs.size;
  }

  static void unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  void clear() {
    if (size) {
      glDeleteFramebuffers(size, &id);
      size = 0;
    }
  }

  void attach2D(GLenum attachment, const Texture2D& tex) const {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex.id, 0);
    unbind();
  }

  void dettach2D(GLenum attachment) const {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
    unbind();
  }
};

