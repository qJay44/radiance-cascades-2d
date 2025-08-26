#include "Shape2D.hpp"

#include "glm/ext/matrix_transform.hpp"

void Shape2D::translate(vec2 v) {
  matTrans = glm::translate(matTrans, vec3(v, 0.f));
}

void Shape2D::rotate(float angleRad) {
  matRot = glm::rotate(matRot, angleRad, vec3(0.f, 0.f, 1.f));
}

void Shape2D::scale(vec2 v) {
  matScale = glm::scale(matScale, vec3(v, 1.f));
}

void Shape2D::draw(const Shader& shader) const {
  vao.bind();

  shader.setUniformMatrix4f("u_model", matTrans * matRot * matScale);
  shader.setUniform3f("u_shapeColor", color);

  glDrawArrays(mode, 0, vertCount);

  vao.unbind();
}

