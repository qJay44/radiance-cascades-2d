#pragma once

#include "utils/utils.hpp"

namespace rc {

extern struct Config {
  float interval0;
  float diagonal;
  float factor;
  float intervalStart;
  u8 cascadeCount;

  u8 drawCascadeIdx = 0;

  void update() {
    vec2 winSize = getWinSize(global::window);

    interval0 = 7.0f;
    diagonal = glm::length(winSize);
    factor = glm::ceil(log4(diagonal / interval0));
    intervalStart = (interval0 * (1.f - glm::pow(4.f, factor))) / (1.f - 4.f);
    cascadeCount = glm::ceil(log4(intervalStart)) - 1;
  }
} config;

} // namespace rc

