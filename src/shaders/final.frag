#version 460 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 shapeColor;

uniform sampler2D u_inputTex;

void main() {
  vec2 uv = texCoord;
  uv.y = 1.f - uv.y;
  FragColor = texture(u_inputTex, uv);
}

