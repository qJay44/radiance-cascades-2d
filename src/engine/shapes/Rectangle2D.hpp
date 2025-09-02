#pragma once

#include "Shape2D.hpp"

// NOTE: Origin is the center of the rectangle

class Rectangle2D final : public Shape2D {
public:
  Rectangle2D();
  Rectangle2D(vec2 size, vec2 pos = vec2(0.f), vec3 color = vec3(0.f));

  bool contains(vec2 mouse) const override;
  void setPosition(vec2 pos) override;

  const vec2& getSize() const;

private:
  static bool allocatedVO;
  static VAO vaoRect;
  static VBO vboRect;

  vec2 size;

private:
  static void initVO();
};

