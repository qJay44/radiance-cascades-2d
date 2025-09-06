#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_sceneTex;

void main() {
  float alpha = texture(u_sceneTex, texCoord).a;
  FragColor = vec4(texCoord * alpha, 0.f, 1.f);
}

