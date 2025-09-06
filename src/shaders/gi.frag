#version 460 core

#define DIAGONAL_LENGTH_NORM 1.41421356237f // sqrt(2.f)

#define PI 3.141592265359f
#define TAU (2.f * PI)


out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_inputTex;
uniform sampler2D u_sdfTex;
uniform vec2 u_resolution;
uniform int u_rayCountBase;
uniform int u_rayCount;
uniform int u_rayMaxSteps;
uniform float u_epsilon;
uniform float u_scale;
uniform float u_srgb;

void main() {
  vec2 coord = ivec2(texCoord * u_resolution);
  vec4 radiance = vec4(0.f);

  bool isLastLayer = u_rayCount == u_rayCountBase;

  // uv of half image (1 of 4 squares)
  vec2 effectiveUV = isLastLayer ? texCoord : floor(coord * 0.5f) * 2.f / u_resolution;

  float partial = 0.125f; // 1/8th of the uv (interval0 ?)
  float intervalStart = isLastLayer ? 0.f : partial;
  float intervalEnd = isLastLayer ? partial : DIAGONAL_LENGTH_NORM;
  float rayCountStepNorm = 1.f / u_rayCount;
  float angleStepSize = TAU * rayCountStepNorm;

  for (int i = 0; i < u_rayCount; i++) {
    float angleStep = float(i) + 0.5f; // Add 0.5 radians to avoid vertical radians?
    float angle = angleStep * angleStepSize;
    vec2 dir = vec2(cos(angle), -sin(angle));

    vec2 sampleUV = effectiveUV + dir * intervalStart * u_scale;
    float traveled = intervalStart;
    vec4 radDelta = vec4(0.f);

    for (int step = 1; step < u_rayMaxSteps; step++) {
      float dist = texture(u_sdfTex, sampleUV).r;

      sampleUV += dir * dist * u_scale;

      if (floor(sampleUV) != vec2(0.f)) break;

      if (dist < u_epsilon) {
        vec4 texel = texture(u_inputTex, sampleUV);
        radDelta += vec4(pow(texel.rgb, vec3(u_srgb)), 1.f);
        break;
      }

      traveled += dist;
      if (traveled >= intervalEnd) break;
    }

    radiance += radDelta;
  }

  vec3 final = radiance.rgb * rayCountStepNorm;
  vec3 correctSRGB = pow(final, vec3(1.f / u_srgb));

  FragColor = vec4(correctSRGB, 1.f);
}

