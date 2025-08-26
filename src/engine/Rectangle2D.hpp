#pragma once

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

class Rectangle2D {
public:
  Rectangle2D(vec2 size, vec2 pos = vec2(0.f));

  void translate(vec2 v);
  void rotate(float angleRad);
  void scale(vec2 v);

  void draw(const Shader& shader) const;

private:
  static bool allocatedVO;
  static VAO vao;
  static VBO vbo;

  mat4 matTrans = mat4(1.f);
  mat4 matRot   = mat4(1.f);
  mat4 matScale = mat4(1.f);

private:
  static void initVO();
};

