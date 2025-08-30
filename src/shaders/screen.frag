#version 460 core

#define PI 3.141592265359f
#define PI_2 (PI * 0.5f)
#define PI_3 (PI / 3.f)
#define PI_6 (PI / 6.f)

#define TAU (2.f * PI)

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_shapesTexture;
uniform sampler2D u_sdfTexture;
uniform uint u_cascadeCount;
uniform uint u_stepsPerRay;
uniform float u_interval0;
uniform float u_epsilon;

ivec2 resolution = textureSize(u_shapesTexture, 0).xy;
vec2 uvStep = 1.f / resolution;

struct Ray {
  vec2 origin;
  vec2 dir;
  float length;
};

vec4 rayMarch(Ray ray) {
  uint steps = uint(ceil(ray.length / u_stepsPerRay));
  float dist = 0.f;
  vec2 pix = ray.origin * uvStep;

  for (uint i = 0; i < steps; i++) {
    dist = texture(u_sdfTexture, pix).r;
    pix += ray.dir * dist;

    if (pix.x > 1.f || pix.x < 0.f || pix.y > 1.f || pix.y < 0.f)
      break;

    if (dist < u_epsilon) {
      return vec4(max(
        texture(u_shapesTexture, pix).rgb,
        texture(u_shapesTexture, pix - (ray.dir * uvStep)).rgb
      ), 0.f);
    }
  }

  return vec4(vec3(0.f), 1.f);
}

float intervalScale(uint cascadeIdx) {
  float val = 0.f;
  if (cascadeIdx)
    val = float(1 << (2 * cascadeIdx));

  return val;
}

vec2 intervalRange(uint cascadeIdx) {
  return u_interval0 * vec2(intervalScale(cascadeIdx), intervalScale(cascadeIdx + 1));
}

void main() {
  ivec2 texSize = resolution;
  vec2 uv = gl_FragCoord.xy / texSize;
  vec3 color = texture(u_shapesTexture, uv).rgb;

  for (uint i = 0; i < u_cascadeCount; i++) {
    uint cascadeRes = uint(pow(2, i));
    uvec2 probSize = texSize / cascadeRes;
    uvec2 probPos = probSize / 2;
    uvec2 dirCoord = uvec2(gl_FragCoord.xy) % probSize;
    uint dirIdx = dirCoord.x + dirCoord.y * probSize.x;
    uint dirCount = probSize.x * probSize.y;

    // +0.5f ?
    float angle = TAU * ((float(dirIdx) + 0.5f) / float(dirCount));
    vec2 dir = vec2(cos(angle), sin(angle));

    vec2 intervalRange = intervalRange(i);
    vec2 intervalStart = probPos + dir * intervalRange.x;
    vec2 intervalEnd   = probPos + dir * intervalRange.y;
    vec3 radiance = rayMarch(Ray(intervalStart, dir, intervalRange.y - intervalRange.x)).rgb;

    color += radiance / (i + 1);
  }

  FragColor = vec4(color, 1.f);
}

