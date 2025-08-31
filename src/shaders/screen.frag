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
uniform vec2 u_resolution;
uniform vec2 u_cascadeSize;  // (u_resolution / u_linear0)
uniform uint u_cascadeCount; // Depends on u_interval0
uniform float u_linear0;     // Cascade0 probe density
uniform float u_interval0;   // Cascade0 ray length
uniform float u_epsilon;     // SDF threshold (how close to 0)

vec2 uvStep = 1.f / u_resolution;

struct ProbeInfo {
  uint cascadeIdx;
  uint angular; // Rays per probe
  vec2 linear;  // Spacing between probes
  uvec2 size;   // Prob size
  vec2 probe;   // Prob's group idx
  vec2 rayPos;  // "Spatial-xy ray-index position"          ???
  float index;  // "PreAvg Index (actual = index * 4)"      ???
  float offset; // The current ray's offset (origin) from the cascade0's origin
  float range;  // The ray's length threshold (max distance)
  float scale;  // The length of the screen's diagonal
};

ProbeInfo cascadeTexelInfo(ivec2 coord, uint cascadeIdx) {
  ProbeInfo info;
  uint angular = uint(pow(2, cascadeIdx));

  info.cascadeIdx = cascadeIdx;
  info.angular = angular * angular;
  info.linear = vec2(u_linear0 * angular);
  info.size = uvec2(u_cascadeSize) / angular;
  info.probe = coord % info.size;
  info.rayPos = floor(texCoord * angular); // Try UV?
  info.index = info.rayPos.x + (angular * info.rayPos.y);
  info.offset = (u_interval0 * (1.f - pow(4, cascadeIdx))) / (1.f - 4.f);
  info.range = u_interval0 * pow(4, cascadeIdx);
  info.range += length(vec2(u_linear0 * pow(2, cascadeIdx + 1))); // Light leak fix
  info.scale = length(u_resolution);

  return info;
}

// Returns color with aplha value representing the visibility term of the hit
vec4 rayMarch(vec2 p, float theta, ProbeInfo probeInfo) {
  vec2 dir = vec2(cos(theta), sin(theta));
  vec2 origin = (p + dir * probeInfo.offset) * uvStep;

  for (float i = 0.f, rayDist = 0.f; i < probeInfo.range; i++) {
    float sdf = texture(u_sdfTexture, origin).r;

    // Increase ray distance by [sdf] amount that scaled from UV to pixel-coordinate (screen coordinates).
    // If [sdf] is 1.0 which means max distance possible, in pixel-coordinate it would distance between diagonal cornes of the screen,
    // (top-left with bottom-right or top-right with bottom-left) so its the length of the screen's diagonal.
    rayDist += sdf * probeInfo.scale;

    // Move towards [dir] by [rayDist] amount, converting to UV range with [uvStep]
    origin += dir * rayDist * uvStep;

    // If ray goes off probe's range or off screen edges, return no-hit
    if (rayDist >= probeInfo.range || floor(origin) != vec2(0.f)) break;

    // The [origin] hit something when [sdf] is very small (accorging to u_epsilon) which means its very close to some shape.
    // [rayDist] may be smaller than u_epsilon when landed on the negative [sdf] value which means its inside some shape.
    // Not sure, but I guess [rayDist] for cascade0 can be very small so we skip the (rayDist <= u_epsilon) condition to properly raymarch through cascade0.
    if (sdf <= u_epsilon && rayDist <= u_epsilon && probeInfo.cascadeIdx != 0) return vec4(0.f);

    // The [origin] hit something, return radiance from scene with visibility term of 0, e.g no visibility to merge with higher cascades
    if (sdf <= u_epsilon) return vec4(texture(u_shapesTexture, origin).rgb, 0.f);
  }

  // No-hit, with visibility term of 1, to merge with higher cascades
  return vec4(vec3(0.f), 1.f);
}

// Merge current cascade with the next highest cascade
vec4 merge(vec4 rayInfo, float idx, ProbeInfo probeInfo) {
  // For any radiance with 0 alpha do not merge (highest cascade also cannot merge)
  if (rayInfo.a == 0.f || probeInfo.cascadeIdx >= u_cascadeCount - 1)
    // Return the radiance with corrected alpha
    return vec4(rayInfo.rgb, 1.f - rayInfo.a);

  // Probe info for cascade N+1
  float angularN1 = pow(2, probeInfo.cascadeIdx + 1);
  vec2 sizeN1 = probeInfo.size * 0.5f;
  vec2 probeN1 = vec2(uint(idx) % uint(angularN1), floor(idx / angularN1)) * sizeN1; // uhhh....

  vec2 interpUVN1 = probeInfo.probe * 0.5f;
  vec2 clampedUVN1 = max(vec2(1.f), min(interpUVN1, sizeN1 - 1.f));
  vec2 probeUVN1 = probeN1 + clampedUVN1 + 0.25f;
  vec4 interpolated = texture(u_shapesTexture, probeUVN1 * (1.f / u_cascadeSize));

  return rayInfo + interpolated;
}

void main() {
  FragColor = vec4(0.f);

  for (uint i = 0; i < u_cascadeCount; i++) {
    ProbeInfo probeInfo = cascadeTexelInfo(ivec2(gl_FragCoord.xy), i);
    vec2 origin = (probeInfo.probe + 0.5f) * probeInfo.linear;
    float preAvgIdx = probeInfo.index * 4.f; // 4x rays cast but averaged to 1x
    float thetaScalar = TAU / (probeInfo.angular * 4.f);

    // Cast 4 rays, one for each angular index for this pre-averaged ray
    for (uint j = 0; j < 4; j++) {
      // Actual index
      float idx = preAvgIdx + j;
      // Actual angle
      float theta = (idx + 0.5f) * thetaScalar;
      vec4 rayInfo = rayMarch(origin, theta, probeInfo);

      // Lookup the 4 rays of cascade N+1 in the same direction as this ray, merge and average the results
      FragColor += merge(rayInfo, idx, probeInfo) * 0.25f;
    }
  }
}

