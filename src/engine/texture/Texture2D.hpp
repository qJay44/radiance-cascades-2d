#pragma once

#include "Texture.hpp"
#include "utils/utils.hpp"

class Texture2D : public Texture {
public:
  Texture2D(const TextureDescriptor& desc, ivec2 size, const u8* pixels = nullptr) : Texture(desc) {
    if (desc.target != GL_TEXTURE_2D)
      error("[Texture2D] Got unexpected texture target ({})", desc.target);

    glTexImage2D(desc.target, 0, desc.internalFormat, size.x, size.y, 0, desc.format, desc.type, pixels);
    unbind();
  }

  void update(const u8* pixels) {
    uvec2 size = getSize();
    bind();
    glTexSubImage2D(desc.target, 0, 0, 0, size.x, size.y, desc.format, desc.type, pixels);
    unbind();
  }

private:
  friend struct FBO;
};

