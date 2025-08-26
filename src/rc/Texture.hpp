#pragma once

#include <cassert>

#include "utils/utils.hpp"

namespace rc {

class Texture {
public:
  Texture(const std::string& uniformName, GLuint unit, u8 cascadeCount)
    : uniformName(uniformName),
      unit(unit),
      cascadeCount(cascadeCount) {

    if (cascadeCount > 10)
      error("[rc::Texture] Trying to create more then 10 cascades ({})", cascadeCount);

    ivec2 winSize = getWinSize(global::window);

    glGenTextures(cascadeCount, &texId);
    bind();
    glTexStorage3D(target, 1, GL_RG8, winSize.x, winSize.y, cascadeCount);

    for (u8 i = 0; i < cascadeCount; i++)
      glTexSubImage3D(target, 0, 0, 0, i, winSize.x, winSize.y, 1, GL_RG, GL_UNSIGNED_BYTE, nullptr);

    unbind();
  }

  const std::string& getUniformName() const {
    return uniformName;
  }

  const GLuint& getUnit() const {
    return unit;
  }

  uvec2 getSize() const {
    ivec2 size;

    bind();
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &size.x);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &size.y);
    unbind();

    assert(size.x >= 0);
    assert(size.y >= 0);

    return size;
  }

  void bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, texId);
  }

  void unbind() const {
    glBindTexture(target, 0);
  }

  void bindImage(GLenum access) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindImageTexture(unit, texId, 0, GL_TRUE, 0, access, GL_RG8);
  }

  void clear() {
    glDeleteTextures(target, &texId);
    cascadeCount = 0;
  }

private:
  const GLenum target = GL_TEXTURE_2D_ARRAY;

  std::string uniformName;
  GLuint unit;
  GLuint texId = 0;
  u8 cascadeCount = 0;
};

} // namespace rc

