#version 460 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 shapeColor;

uniform sampler2D sdfTexture;

void main() {
  FragColor = vec4(shapeColor, 1.f);
}

