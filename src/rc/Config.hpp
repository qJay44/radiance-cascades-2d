#pragma once

#include "utils/utils.hpp"
#include "CascadesTexture.hpp"

namespace rc {

extern struct Config {
  float interval0;
  float diagonal;
  float factor;
  float intervalStart;
  float stepsPerRay = 32;
  u8 cascadeCount;
  u8 drawCascadeIdx = 0;
  CascadesTexture cascadesTex;

  void init(float intervalInit, GLuint texUnit) {
    interval0 = intervalInit;
    update();
    cascadesTex = CascadesTexture("u_rcTexture", texUnit, cascadeCount);
  }

  void update() {
    vec2 winSize = getWinSize(global::window);

    diagonal = glm::length(winSize);
    factor = glm::ceil(log4(diagonal / interval0));
    intervalStart = (interval0 * (1.f - glm::pow(4.f, factor))) / (1.f - 4.f);
    cascadeCount = glm::ceil(log4(intervalStart)) - 1;
  }
} config;

} // namespace rc

