#include "Circle2D.hpp"

Circle2D::Circle2D(float radius, vec2 pos, vec3 color, size_t points) {
  vertices.push_back({vec2(0.f), vec2(0.5f)});

  const float angleStep = TAU / points;

  for (size_t point = 0; point < points + 1; point++) {
    float angle = point * angleStep;
    vec2 radDir = vec2(cos(angle), sin(angle));
    vec2 texCoord = radDir * 0.5f + 0.5f; // REVIEW: Is it correct?
    vertices.push_back({radDir, texCoord});
  }

  mode = GL_TRIANGLE_FAN;
  vertCount = vertices.size();
  Shape2D::color = color;
  vao = VAO(1);
  vbo = VBO(1, vertices.data(), vertCount * sizeof(Vertex2D));

  vao.bind();
  vbo.bind();

  GLsizei stride = sizeof(Vertex2D);

  vao.linkAttrib(0, 2, GL_FLOAT, stride, (void*)(0 * sizeof(float)));
  vao.linkAttrib(1, 2, GL_FLOAT, stride, (void*)(2 * sizeof(float)));

  vao.unbind();
  vbo.unbind();

  vec2 winSize = getWinSize(global::window);
  translate(pos - winSize * 0.5f);
  scale(radius / winSize);
}

