#version 460 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_tex;

out vec2 texCoord;
out vec3 shapeColor;

uniform mat4 u_model;
uniform vec3 u_shapeColor;

void main() {
  texCoord = in_tex;
  shapeColor = u_shapeColor;
  gl_Position = u_model * vec4(in_pos, 0.f, 1.f);
}

