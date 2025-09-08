#version 460 core

#define DIAGONAL_LENGTH_NORM 1.41421356237f // sqrt(2.f)

#define PI 3.141592265359f
#define TAU (2.f * PI)


out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_sceneTex;
uniform sampler2D u_sdfTex;
uniform sampler2D u_inputTex; // last pass cascade (from here)
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

  // does it really matter?
  vec2 effectiveUV = isLastLayer ? texCoord : (floor(coord * 0.5f) * 2.f) / u_resolution;

  float intervalStart = isLastLayer ? 0.f : u_interval0;
  float intervalEnd = isLastLayer ? u_interval0 : DIAGONAL_LENGTH_NORM;
  float rayCountStepNorm = 1.f / u_rayCount;
  float minStepSize = min(uvStep.x, uvStep.y) * 0.5f;

  float sqrtBase = sqrt(u_rayCountBase);
  float spacing = isLastLayer ? 1.f : sqrtBase; // last cascade - one probe; first cascade - 4 probes (u_rayCountBase = 16)
  vec2 size = floor(u_resolution / spacing);
  vec2 probeRelativePosition = mod(coord, size);
  vec2 rayPos = floor(coord / size);
  float baseIdx = float(u_rayCountBase) * (rayPos.x + (spacing * rayPos.y));
  float angleStepSize = TAU * rayCountStepNorm;
  vec2 probeCenter = (probeRelativePosition + 0.5f) * spacing;

  for (int i = 0; i < u_rayCountBase; i++) {
    float index = baseIdx + float(i);
    float angleStep = index + 0.5f; // Add 0.5 radians to avoid vertical/horizontal (90°) rays
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

      // Only merge on non-opaque areas (no radiance)
      if (isLastLayer && radDelta.a == 0.f) {
       // This whole block, only for 2 cascades case?
        vec2 upperSpacing = vec2(sqrtBase);
        vec2 upperSize = floor(u_resolution / upperSpacing);
        vec2 upperPosition = vec2(mod(index, sqrtBase), floor(index) / upperSpacing) * upperSize;
        vec2 offset = (probeRelativePosition + 0.5f) / sqrtBase;
        vec2 upperUV = (upperPosition + offset) / u_resolution;
        radDelta += texture(u_inputTex, upperUV);
      }
    }

    radiance += radDelta;
  }

  vec3 final = radiance.rgb * rayCountStepNorm;
  vec3 correctSRGB = pow(final, vec3(1.f / u_srgb));

  FragColor = vec4(correctSRGB, 1.f);
}

