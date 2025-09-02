#pragma once

#include "../RenderConfig.hpp"

struct gui {
  static RenderConfig* renderConfig;

  static void toggle();
  static void draw();
};

