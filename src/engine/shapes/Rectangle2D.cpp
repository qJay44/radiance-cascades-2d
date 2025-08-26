#include "Rectangle2D.hpp"

#include <vector>

#include "Vertex2D.hpp"

static const std::vector<Vertex2D> vertices{
  // Triangle 1
  {{-1.f, -1.f}, {0.f, 0.f}},
  {{-1.f,  1.f}, {0.f, 1.f}},
  {{ 1.f,  1.f}, {1.f, 1.f}},
  // Triangle 2
  {{ 1.f,  1.f}, {1.f, 1.f}},
  {{ 1.f, -1.f}, {1.f, 0.f}},
  {{-1.f, -1.f}, {0.f, 0.f}},
};

bool Rectangle2D::allocatedVO = false;
VAO Rectangle2D::vaoRect;
VBO Rectangle2D::vboRect;

void Rectangle2D::initVO() {
  vaoRect = VAO(1);
  vboRect = VBO(1, vertices.data(), vertices.size() * sizeof(Vertex2D));

  vaoRect.bind();
  vboRect.bind();

  GLsizei stride = sizeof(Vertex2D);

  vaoRect.linkAttrib(0, 2, GL_FLOAT, stride, (void*)(0 * sizeof(float)));
  vaoRect.linkAttrib(1, 2, GL_FLOAT, stride, (void*)(2 * sizeof(float)));

  vaoRect.unbind();
  vboRect.unbind();

  allocatedVO = true;
}

Rectangle2D::Rectangle2D(vec2 size, vec2 pos, vec3 color) {
  if (!allocatedVO)
    initVO();

  mode = GL_TRIANGLES;
  vertCount = vertices.size();
  vao = vaoRect;
  vbo = vboRect;
  Shape2D::color = color;

  translate(pos);
  scale(size / vec2(getWinSize(global::window)));
}

