#pragma once

#include "TextureDescriptor.hpp"

class Texture {
public:
  Texture();
  Texture(const TextureDescriptor& desc); // Only generates and binds texture

  const std::string& getUniformName() const;
  const GLuint& getUnit() const;
  const GLuint& getId() const;
  uvec2 getSize() const;

  void bind() const;
  void unbind() const;

  void bindImage(GLenum access) const;

  void clear();

protected:
  TextureDescriptor desc;
  GLuint id = 0;
};

