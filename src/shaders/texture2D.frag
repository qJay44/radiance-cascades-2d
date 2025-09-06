#version 460 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 shapeColor;

uniform sampler2D u_texture;

void main() {
  FragColor = vec4(texture(u_texture, texCoord).rgb, 1.f);
}

