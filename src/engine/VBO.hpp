#pragma once

#include "glObject.hpp"

// Vertex Buffer Object
struct VBO final : glObject {
  void gen() override {
    glGenBuffers(1, &id);
  }

  void bind() const override {
    glBindBuffer(GL_ARRAY_BUFFER, id);
  }

  void unbind() const override {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void destroy() override {
    if (id) {
      glDeleteBuffers(1, &id);
      id = 0;
    }
  }

  VBO& operator=(VBO&& rhs) noexcept = default;

  VBO() = default;

  ~VBO() {
    destroy();
  }

  void store(const void* data, GLsizeiptr dataSize)  {
    bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    unbind();
  }
};

