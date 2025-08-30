#pragma once

#include <vector>

#include "engine/Shader.hpp"
#include "engine/shapes/Circle2D.hpp"
#include "engine/shapes/Rectangle2D.hpp"
#include "utils/utils.hpp"

struct ShapeContainer {
  std::vector<Circle2D> circles;
  std::vector<Rectangle2D> rects;
  Shape2D* grabbedShape = nullptr;

  ShapeContainer() {
    ivec2 winSize = getWinSize(global::window);

    circles.push_back(Circle2D(90.f, winSize / 2, {1.f, 0.f, 1.f}));
    rects.push_back(Rectangle2D({300.f, 100.f}, {100.f, 100.f}, randColor255Norm()));
  }

  void moveShape(vec2 mouse, bool hold) {
    if (!grabbedShape && hold) {
      for (Circle2D& circle : circles) {
        if (circle.contains(mouse)) {
          grabbedShape = &circle;
          break;
        }
      }

      for (Rectangle2D& rect : rects) {
        if (rect.contains(mouse)) {
          grabbedShape = &rect;
          break;
        }
      }
    }

    if (grabbedShape) {
      grabbedShape->setPosition(mouse);

      if (!hold)
        grabbedShape = nullptr;
    }
  }

  void draw(const Shader& shader) const {
    for (const auto& circle : circles) circle.draw(shader);
    for (const auto& rect : rects) rect.draw(shader);
  }
};

