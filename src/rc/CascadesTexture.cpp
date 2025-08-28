#include "CascadesTexture.hpp"

#include <cassert>

#include "utils/utils.hpp"

namespace rc {

CascadesTexture::CascadesTexture() {}

CascadesTexture::CascadesTexture(const std::string& uniformName, GLuint unit, u8 cascadeCount)
  : Texture({uniformName, unit, GL_TEXTURE_2D_ARRAY, GL_RG, GL_RG}) {

  if (cascadeCount > 10)
    error("[rc::CascadesTexture] Trying to create more then 10 cascades ({})", cascadeCount);

  ivec2 winSize = getWinSize(global::window);

  glTexStorage3D(desc.target, 1, GL_RG8, winSize.x, winSize.y, cascadeCount);

  for (u8 i = 0; i < cascadeCount; i++)
    glTexSubImage3D(desc.target, 0, 0, 0, i, winSize.x, winSize.y, 1, GL_RG, GL_UNSIGNED_BYTE, nullptr);

  unbind();
}

} // namespace rc

