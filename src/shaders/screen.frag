#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2DArray u_rcTexture;
uniform uint u_cascadeIdx;

void main() {
  vec3 color = vec3(texture(u_rcTexture, vec3(texCoord, u_cascadeIdx)).rg, 0.f);
  FragColor = vec4(color, 1.f);
}

