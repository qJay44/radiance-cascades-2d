#pragma once

#include "ProfilerManager.hpp"
#include "engine/Shader.hpp"
#include "engine/RenderTexture2D.hpp"
#include "engine/shapes/Rectangle2D.hpp"
#include "utils/types.hpp"

class RenderConfig {
public:
  RenderConfig(ProfilerManager* profilerManager);

  void init(uvec2 winSize);
  void addProfilier(ProfilerManager* pm);
  void update();
  void drawGI();

  void onMousePressed(const vec2& pos);
  void onMouseReleased();
  void onMouseMoved(const vec2& pos);

private:
  friend struct gui;

  uvec2 winSize;

  RenderTexture2D sceneTexture;
  RenderTexture2D seedTexture;
  RenderTexture2D pingJFA;
  RenderTexture2D pongJFA;
  RenderTexture2D sdfTexture;
  RenderTexture2D* jfaTex = &pongJFA;

  Rectangle2D screenRect;

  Shader seedShader  = Shader("default2D.vert", "seed.frag");
  Shader jfaShader   = Shader("default2D.vert", "jfa.frag");
  Shader sdfShader   = Shader("default2D.vert", "sdf.frag");
  Shader giShader    = Shader("default2D.vert", "gi.frag");
  Shader tex2DShader = Shader("default2D.vert", "texture2D.frag");
  Shader mouseShader = Shader("default2D.vert", "mouse.frag");

  // Mouse
  float mouseRadius = 5.f;
  vec3 mouseColor{1.f, 0.f, 1.f};
  vec2 mousePrevPos;

  // Global illumination
  int raysPerPixel = 32;
  int stepsPerRay = 32;
  float epsilon = 0.001f;
  int jfaPasses = 1;

  bool isDrawing = false;
  bool autoJfaPasses = true;

  ProfilerManager* profilerManager = nullptr;

private:
  void calcPassesJFA();
  void drawMouseAt(const vec2& point);
  void drawSeed();
  void drawJFA();
  void drawSDF();
};

