#pragma once

#include "../Shader.hpp"

#include "VAO.hpp"
#include "VBO.hpp"

class Shape2D {
public:
  void translate(vec2 v);
  void rotate(float angleRad);
  void scale(vec2 v);

  void draw(const Shader& shader) const;

protected:
  GLenum mode;
  GLsizei vertCount;
  VAO vao;
  VBO vbo;
  vec3 color;

  mat4 matTrans = mat4(1.f);
  mat4 matRot   = mat4(1.f);
  mat4 matScale = mat4(1.f);
};

