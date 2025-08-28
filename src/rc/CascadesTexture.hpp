#pragma once

#include "../engine/texture/Texture.hpp"

namespace rc {

class CascadesTexture : public Texture {
public:
  CascadesTexture();
  CascadesTexture(const std::string& uniformName, GLuint unit, u8 cascadeCount);
};

} // namespace rc

