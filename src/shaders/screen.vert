#version 460 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec3 in_col;
layout(location = 2) in vec2 in_tex;

out vec2 texCoord;

uniform mat4 u_model;

void main() {
  texCoord = in_tex;
  gl_Position = u_model * vec4(in_pos, 1.f, 1.f);
}

