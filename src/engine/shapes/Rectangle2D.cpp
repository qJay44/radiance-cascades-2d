#include "Rectangle2D.hpp"

#include <vector>

#include "../Vertex2D.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils/utils.hpp"

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

Rectangle2D::Rectangle2D() {
  if (!allocatedVO)
    initVO();
}

Rectangle2D::Rectangle2D(vec2 size, vec2 pos, vec3 color)
  : size(size),
    pos(pos),
    color(color) {

  if (!allocatedVO)
    initVO();

  vec2 winSize = getWinSize(global::window);
  vec2 sizeNorm = size / winSize;

  setPosition(pos);
  scale(sizeNorm);
}

const vec2& Rectangle2D::getSize() const {
  return size;
}

void Rectangle2D::setPosition(vec2 pos) {
  vec2 winSize = vec2(getWinSize(global::window));
  vec2 toTranslate = pos / winSize * 2.f - 1.f;
  toTranslate.y *= -1.f;
  matTrans = glm::translate(mat4(1.f), vec3(toTranslate, 0.f));
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
  vaoRect.bind();

  shader.setUniformMatrix4f("u_model", matTrans * matRot * matScale);
  shader.setUniform3f("u_shapeColor", color);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  vaoRect.unbind();
}

