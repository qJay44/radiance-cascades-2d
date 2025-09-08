#pragma once

struct glObject {
  GLuint id = 0;

  virtual void gen() = 0;
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void destroy() = 0;

  glObject(const glObject&) = delete;
  glObject& operator=(const glObject&) = delete;

  glObject(glObject&& rhs) noexcept : id(rhs.id) {
    rhs.id = 0;
  }

  glObject& operator=(glObject&& rhs) noexcept {
    if (this != &rhs) {
      destroy();
      id = rhs.id;
      rhs.id = 0;
    }
    return *this;
  }

  glObject() = default;
  ~glObject() = default;
};

