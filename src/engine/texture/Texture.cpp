#include "Texture.hpp"

void Texture::unbind(GLenum target) {
  glBindTexture(target, 0);
}

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

void Texture::operator=(Texture rhs) {
  glDeleteTextures(desc.target, &id);
  desc = rhs.desc;
  id = rhs.id;
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

const uvec2& Texture::getSize() const {
  return desc.size;
}

uvec2 Texture::getSizeNative() const {
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
  unbind(desc.target);
}

void Texture::bindImage(GLenum access) const {
  glActiveTexture(GL_TEXTURE0 + desc.unit);
  glBindImageTexture(desc.unit, id, 0, GL_TRUE, 0, access, GL_RG8);
}

