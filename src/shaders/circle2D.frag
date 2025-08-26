#version 460 core

out vec4 FragColor;

in vec3 shapeColor;

void main() {
  FragColor = vec4(shapeColor, 1.f);
}

