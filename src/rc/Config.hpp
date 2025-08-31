#pragma once

#include "utils/utils.hpp"
#include "../engine/Shader.hpp"

namespace rc {

extern struct Config {
  float linear0 = 1.f;
  float interval0 = 7.f;
  float epsilon = 1e-5f;
  u8 cascadeCount = 0;

  void calcCascadeCount() {
    vec2 winSize = getWinSize(global::window);
    float diagonal = glm::length(winSize);
    float factor = glm::ceil(log4(diagonal / interval0));
    float intervalStart = (interval0 * (1.f - glm::pow(4.f, factor))) / (1.f - 4.f);

    cascadeCount = glm::ceil(log4(intervalStart)) - 1;
  }

  void update(const Shader& shader) {
    vec2 winSize = getWinSize(global::window);

    shader.setUniform2f("u_resolution", winSize);
    shader.setUniform2f("u_cascadeSize", winSize / interval0);
    shader.setUniform1ui("u_cascadeCount", cascadeCount);
    shader.setUniform1f("u_linear0", linear0);
    shader.setUniform1f("u_interval0", interval0);
    shader.setUniform1f("u_epsilon", epsilon);
  }

} config;

} // namespace rc

