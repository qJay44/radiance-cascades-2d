#pragma once

#include "Texture.hpp"
#include "utils/utils.hpp"
#include <cassert>

class Texture2D : public Texture {
public:
  Texture2D() = default;
  ~Texture2D() = default;

  Texture2D& operator=(Texture2D&& rhs) = default;

  template<typename T = u8>
  Texture2D(const TextureDescriptor& desc, const T* pixels = nullptr) : Texture(desc) {
    if (desc.target != GL_TEXTURE_2D)
      error("[Texture2D] Got unexpected texture target ({})", desc.target);

    if (pixels)
      checkPacking<T>();

    glTexImage2D(desc.target, 0, desc.internalFormat, desc.size.x, desc.size.y, 0, desc.format, desc.type, pixels);
    unbind();
  }

  template<typename T>
  void update(const T* pixels) {
    assert(pixels != nullptr);

    [[maybe_unused]]
    uvec2 sizeNative = getSizeNative();
    assert(sizeNative.x == desc.size.x);
    assert(sizeNative.y == desc.size.y);

    checkPacking<T>();

    bind();
    glTexSubImage2D(desc.target, 0, 0, 0, desc.size.x, desc.size.y, desc.format, desc.type, pixels);
    unbind();
  }

private:
  friend struct FBO;
};

