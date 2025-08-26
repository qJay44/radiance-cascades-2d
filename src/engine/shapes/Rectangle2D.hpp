#pragma once

#include "Shape2D.hpp"

class Rectangle2D : public Shape2D {
public:
  Rectangle2D(vec2 size, vec2 pos = vec2(0.f), vec3 color = vec3(0.f));

private:
  static bool allocatedVO;
  static VAO vaoRect;
  static VBO vboRect;

private:
  static void initVO();
};

