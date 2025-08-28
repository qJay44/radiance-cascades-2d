#include "Texture.hpp"

Texture::Texture() {}

Texture::Texture(const TextureDescriptor& desc) : desc(desc) {
  glGenTextures(1, &id);
  bind();
  glTexParameteri(desc.target, GL_TEXTURE_MIN_FILTER, desc.minFilter);
  glTexParameteri(desc.target, GL_TEXTURE_MAG_FILTER, desc.magFilter);
  glTexParameteri(desc.target, GL_TEXTURE_WRAP_S, desc.wrapS);
  glTexParameteri(desc.target, GL_TEXTURE_WRAP_T, desc.wrapT);
  glTexParameteri(desc.target, GL_TEXTURE_WRAP_R, desc.wrapR);
  // Derived class implementation ...
}

const std::string& Texture::getUniformName() const {
  return desc.uniformName;
}

const GLuint& Texture::getUnit() const {
  return desc.unit;
}

const GLuint& Texture::getId() const {
  return id;
}

uvec2 Texture::getSize() const {
  ivec2 size;

  bind();
  glGetTexLevelParameteriv(desc.target, 0, GL_TEXTURE_WIDTH, &size.x);
  glGetTexLevelParameteriv(desc.target, 0, GL_TEXTURE_HEIGHT, &size.y);
  unbind();

  assert(size.x >= 0);
  assert(size.y >= 0);

  return size;
}

void Texture::bind() const {
  glActiveTexture(GL_TEXTURE0 + desc.unit);
  glBindTexture(desc.target, id);
}

void Texture::unbind() const {
  glBindTexture(desc.target, 0);
}

void Texture::bindImage(GLenum access) const {
  glActiveTexture(GL_TEXTURE0 + desc.unit);
  glBindImageTexture(desc.unit, id, 0, GL_TRUE, 0, access, GL_RG8);
}

void Texture::clear() {
  glDeleteTextures(desc.target, &id);
}

