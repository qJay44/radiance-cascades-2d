#pragma once

#include <vector>

#include "Shape2D.hpp"
#include "Vertex2D.hpp"

class Circle2D : public Shape2D {
public:
  Circle2D(float radius, vec2 pos, vec3 color = vec3(0.f), size_t points = 50);

  vec2 getPosition() const override;

  const float& getRadius() const;

private:
  float radius;
  std::vector<Vertex2D> vertices;
};

