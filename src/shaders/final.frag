#version 460 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 shapeColor;

uniform sampler2D u_texture;

void main() {
  vec2 uv = texCoord;
  uv.y = 1.f - uv.y;
  FragColor = texture(u_texture, uv);
}

