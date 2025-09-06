#version 460 core

in vec2 texCoord;

uniform sampler2D u_jfaTex;

void main() {
  vec2 nearestSeed = texture(u_jfaTex, texCoord).rg;
  float dist = clamp(distance(texCoord, nearestSeed), 0.f, 1.f);

  gl_FragColor = vec4(vec3(dist), 1.f);
}

