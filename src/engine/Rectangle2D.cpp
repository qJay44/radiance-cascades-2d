#include "Rectangle2D.hpp"

#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "Vertex2D.hpp"

static const std::vector<Vertex2D> vertices{
  // Triangle 1
  {{-1.f, -1.f}, {}, {0.f, 0.f}},
  {{-1.f,  1.f}, {}, {0.f, 1.f}},
  {{ 1.f,  1.f}, {}, {1.f, 1.f}},
  // Triangle 2
  {{ 1.f,  1.f}, {}, {1.f, 1.f}},
  {{ 1.f, -1.f}, {}, {1.f, 0.f}},
  {{-1.f, -1.f}, {}, {0.f, 0.f}},
};

bool Rectangle2D::allocatedVO = false;
VAO Rectangle2D::vao;
VBO Rectangle2D::vbo;

void Rectangle2D::initVO() {
  vao = VAO(1);
  vbo = VBO(1, vertices.data(), vertices.size() * sizeof(Vertex2D));

  vao.bind();
  vbo.bind();

  GLsizei stride = sizeof(Vertex2D);

  vao.linkAttrib(0, 2, GL_FLOAT, stride, (void*)(0 * sizeof(float)));
  vao.linkAttrib(1, 3, GL_FLOAT, stride, (void*)(2 * sizeof(float)));
  vao.linkAttrib(2, 2, GL_FLOAT, stride, (void*)(5 * sizeof(float)));

  vao.unbind();
  vbo.unbind();

  allocatedVO = true;
}

Rectangle2D::Rectangle2D(vec2 size, vec2 pos) {
  if (!allocatedVO)
    initVO();

  translate(pos);
  scale(size / vec2(getWinSize(global::window)));
}

void Rectangle2D::translate(vec2 v) {
  matTrans = glm::translate(matTrans, vec3(v, 0.f));
}

void Rectangle2D::rotate(float angleRad) {
  matRot = glm::rotate(matRot, angleRad, vec3(0.f, 0.f, 1.f));
}

void Rectangle2D::scale(vec2 v) {
  matScale = glm::scale(matScale, vec3(v, 1.f));
}

void Rectangle2D::draw(const Shader& shader) const {
  vao.bind();

  shader.setUniformMatrix4f("u_model", matTrans * matRot * matScale);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

