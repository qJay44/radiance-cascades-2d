#pragma once

// Vertex Buffer Object
struct VBO {
  GLuint id = 0;
  GLsizei size = 0;

  VBO() = default;

  VBO(GLsizei size, const void* data, GLsizeiptr dataSize) : size(size) {
    glGenBuffers(size, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  }

  void operator=(VBO rhs) {
    clear();
    id = rhs.id;
    size = rhs.size;
  }

  static void unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
  }

  void clear() {
    if (size) {
      glDeleteBuffers(size, &id);
      size = 0;
    }
  }
};

