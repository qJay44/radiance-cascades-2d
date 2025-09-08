#pragma once

#include "glObject.hpp"
#include "texture/Texture2D.hpp"
#include <cstdio>

struct FBO final : glObject {
  void gen() override {
    glGenFramebuffers(1, &id);
  }

  void bind() const override {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  void unbind() const override {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void destroy() override {
    if (id) {
      glDeleteFramebuffers(1, &id);
      id = 0;
    }
  }

  static void Default() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  FBO& operator=(FBO&& rhs) noexcept = default;

  FBO() = default;

  ~FBO() {
    destroy();
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

