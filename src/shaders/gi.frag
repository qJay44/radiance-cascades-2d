#version 460 core

#define DIAGONAL_LENGTH_NORM 1.41421356237f // sqrt(2.f)

#define PI 3.141592265359f
#define TAU (2.f * PI)


out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_sceneTex;
uniform sampler2D u_sdfTex;
uniform sampler2D u_inputTex;
uniform vec2 u_resolution;
uniform int u_rayCountBase;
uniform int u_rayCount;
uniform int u_rayMaxSteps;
uniform float u_interval0;
uniform float u_scale;
uniform float u_srgb;

void main() {
  vec2 coord = ivec2(texCoord * u_resolution);
  vec4 radiance = vec4(0.f);
  vec2 uvStep = 1.f / u_resolution;

  bool isLastLayer = u_rayCount == u_rayCountBase;

  // uv of half image (1 of 4 squares)
  vec2 effectiveUV = isLastLayer ? texCoord : (floor(coord * 0.5f) * 2.f) / u_resolution;

  float intervalStart = isLastLayer ? 0.f : u_interval0;
  float intervalEnd = isLastLayer ? u_interval0 : DIAGONAL_LENGTH_NORM;
  float rayCountStepNorm = 1.f / u_rayCount;
  float angleStepSize = TAU * rayCountStepNorm;
  float minStepSize = min(uvStep.x, uvStep.y) * 0.5f;

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

      if (dist < minStepSize) {
        vec4 texel = texture(u_sceneTex, sampleUV);
        radDelta += vec4(pow(texel.rgb, vec3(u_srgb)), 1.f);
        break;
      }

      traveled += dist;
      if (traveled >= intervalEnd) break;

      if (isLastLayer && radDelta.a == 0.f) {
        vec4 upperSample = texture(u_inputTex, texCoord);
        radDelta += vec4(pow(upperSample.rgb, vec3(u_srgb)), upperSample.a);
      }
    }

    radiance += radDelta;
  }

  vec3 final = radiance.rgb * rayCountStepNorm;
  vec3 correctSRGB = pow(final, vec3(1.f / u_srgb));

  FragColor = radiance * rayCountStepNorm;
}

