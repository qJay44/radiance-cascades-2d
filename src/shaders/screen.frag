#version 460 core

#define PI 3.141592265359f
#define PI_2 (PI * 0.5f)
#define PI_3 (PI / 3.f)
#define PI_6 (PI / 6.f)

#define TAU (2.f * PI)

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2DArray u_rcTexture;
uniform sampler2D u_shapesTexture;
uniform sampler2D u_sdfTexture;
uniform uint u_cascadeIdx;
uniform uint u_stepsPerRay;
uniform float u_interval0;

vec3 rayMarch(vec2 pix, vec2 dir, float rayLength) {
  uint steps = uint(ceil(rayLength / u_stepsPerRay));
  float dist = 0.f;

  for (uint i = 0; i < steps; i++) {
    dist = texture(u_sdfTexture, pix).r;
    pix += dir * dist;

    if (pix.x > 1.f || pix.x < 0.f || pix.y > 1.f || pix.y < 0.f)
      break;

    if (dist < 0.0001f) {
      return texture(u_shapesTexture, pix).rgb;
    }

  }

  return vec3(0.f);
}

void main() {
  ivec2 texSize = textureSize(u_rcTexture, 0).xy;
  vec2 uv = gl_FragCoord.xy / texSize;
  vec3 color = vec3(0.f);

  for (uint i = 0; i < 4; i++) {
    uint cascadeRes = uint(pow(2, u_cascadeIdx + i));
    uint probeSize = uint(texSize / cascadeRes);
    uvec2 probe = uvec2(gl_FragCoord.xy) % probeSize;
    uint rayIdx = probe.y * probeSize + probe.x;
    uint rayCount = probeSize * probeSize;
    float rayAngle = float(rayIdx) / rayCount * TAU;
    vec2 rayDir = vec2(cos(rayAngle), sin(rayAngle));

    float cascadeRes4 = pow(4, u_cascadeIdx + i);
    float origin = (u_interval0 * (1.f - cascadeRes4)) / (1.f - 4.f);
    float rayLength = u_interval0 * cascadeRes4;

    vec3 light = rayMarch(uv, rayDir, rayLength);
    color += light / (i + 1);
  }

  // test textures
  // color = vec3(texture(u_rcTexture, vec3(uv, u_cascadeIdx)).rg, 0.f) * float(u_cascadeIdx == 0) + color;
  // color = texture(u_shapesTexture, uv).rgb * float(u_cascadeIdx == 1) + color;
  // color = vec3(texture(u_sdfTexture, uv).r) * float(u_cascadeIdx == 2) + color;

  FragColor = vec4(color, 1.f);
}

