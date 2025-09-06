#pragma once

#include "TextureDescriptor.hpp"

class Texture {
public:
  static void unbind(GLenum target);

  Texture() = default;
  Texture(const TextureDescriptor& desc); // Only generates and binds texture

  void operator=(Texture rhs);

  const std::string& getUniformName() const;
  const GLuint& getUnit() const;
  const GLuint& getId() const;
  const uvec2& getSize() const;
  uvec2 getSizeNative() const;

  void bind() const;
  void unbind() const;

  void bindImage(GLenum access) const;

  template<typename T>
  void clear(const TextureDescriptor& clearDesc, T* clearValue) {
    glClearTexImage(id, 0, clearDesc.format, clearDesc.type, clearValue);
  }

protected:
  TextureDescriptor desc;
  GLuint id = 0;

protected:
  template<typename T>
  void checkPacking() {
    size_t rowSize = desc.size.x * sizeof(T);
    glPixelStorei(GL_UNPACK_ALIGNMENT, (rowSize % 4 == 0) ? 4 : 1);
  }
};

