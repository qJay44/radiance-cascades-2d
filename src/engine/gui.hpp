#pragma once

#include "../ShapeContainer.hpp"

struct gui {
  static ShapeContainer* shapeContainer;

  static void toggle();
  static void draw();
};

