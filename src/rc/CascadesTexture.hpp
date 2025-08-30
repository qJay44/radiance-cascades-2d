#pragma once

#include "../engine/texture/Texture.hpp"
#include "../engine/Shader.hpp"

namespace rc {

class CascadesTexture : public Texture {
public:
  CascadesTexture();
  CascadesTexture(const std::string& uniformName, GLuint unit, u8 cascadeCount, const Shader& compShader);
};

} // namespace rc

