#include "CascadesTexture.hpp"

#include <cassert>

#include "utils/utils.hpp"

namespace rc {

CascadesTexture::CascadesTexture() {}

CascadesTexture::CascadesTexture(const std::string& uniformName, GLuint unit, u8 cascadeCount, const Shader& compShader)
  : Texture({uniformName, unit, GL_TEXTURE_2D_ARRAY, GL_RG, GL_RG}) {

  assert(cascadeCount < 20);

  ivec2 winSize = getWinSize(global::window);

  glTexStorage3D(desc.target, 1, GL_RG8, winSize.x, winSize.y, cascadeCount);

  for (u8 i = 0; i < cascadeCount; i++)
    glTexSubImage3D(desc.target, 0, 0, 0, i, winSize.x, winSize.y, 1, GL_RG, GL_UNSIGNED_BYTE, nullptr);

  constexpr uvec2 localSize(16);
  const uvec2 numGroups = (uvec2(winSize) + localSize - 1u) / localSize;

  compShader.use();
  bindImage(GL_WRITE_ONLY);
  glDispatchCompute(numGroups.x, numGroups.y, cascadeCount);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  unbind();
}

} // namespace rc

