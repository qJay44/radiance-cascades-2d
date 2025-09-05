#pragma once

#include "../VAO.hpp"
#include "../VBO.hpp"
#include "../Shader.hpp"

// NOTE: Origin is the center of the rectangle

class Rectangle2D {
public:
  Rectangle2D();
  Rectangle2D(vec2 size, vec2 pos = vec2(0.f), vec3 color = vec3(0.f));

  const vec2& getSize() const;

  void setPosition(vec2 pos);

  void translate(vec2 v);
  void rotate(float angleRad);
  void scale(vec2 v);

  void draw(const Shader& shader) const;

private:
  static bool allocatedVO;
  static VAO vaoRect;
  static VBO vboRect;

  vec2 size;
  vec2 pos;
  vec3 color;

  mat4 matTrans = mat4(1.f);
  mat4 matRot   = mat4(1.f);
  mat4 matScale = mat4(1.f);

private:
  static void initVO();
};

