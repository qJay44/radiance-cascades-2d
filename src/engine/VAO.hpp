#pragma once

#include "glObject.hpp"

// Vertex Array Object
struct VAO final : glObject {
  void gen() override {
    glGenVertexArrays(1, &id);
  }

  void bind() const override {
    glBindVertexArray(id);
  }

  void unbind() const override {
    glBindVertexArray(0);
  }

  void destroy() override {
    if (id) {
      glDeleteVertexArrays(1, &id);
      id = 0;
    }
  }

  VAO& operator=(VAO&& rhs) noexcept = default;

  VAO() = default;

  ~VAO() {
    destroy();
  }

  void linkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset) const {
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  }
};

