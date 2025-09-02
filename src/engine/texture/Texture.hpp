#pragma once

#include "TextureDescriptor.hpp"

class Texture {
public:
  Texture();
  Texture(const TextureDescriptor& desc); // Only generates and binds texture

  const std::string& getUniformName() const;
  const GLuint& getUnit() const;
  const GLuint& getId() const;
  const uvec2& getSize() const;
  uvec2 getSizeNative() const;

  void bind() const;
  void unbind() const;

  void bindImage(GLenum access) const;

  void clear();

protected:
  TextureDescriptor desc;
  GLuint id = 0;

protected:
  template<typename T>
  void checkPacking(const T* pixels) {
    assert(pixels != nullptr);

    size_t rowSize = desc.size.x * sizeof(T);
    glPixelStorei(GL_UNPACK_ALIGNMENT, (rowSize % 4 == 0) ? 4 : 1);
  }
};

