#pragma once

#include <vector>

#include "Shape2D.hpp"
#include "Vertex2D.hpp"

class Circle2D final : public Shape2D {
public:
  Circle2D(float radius, vec2 pos, vec3 color = vec3(0.f), size_t points = 50);

  bool contains(vec2 mouse) const override;
  void setPosition(vec2 pos) override;

  const float& getRadius() const;

private:
  float radius;
  std::vector<Vertex2D> vertices;
};

