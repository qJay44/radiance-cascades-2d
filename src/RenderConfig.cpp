#include "RenderConfig.hpp"
#include "ScopeProfileTask.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

RenderConfig::RenderConfig(ProfilerManager* profilerManager) {
  this->profilerManager = profilerManager;
}

void RenderConfig::init(uvec2 winSize) {
  this->winSize = winSize;

  sceneTexture = RenderTexture2D({"u_sceneTex", winSize, 0, GL_TEXTURE_2D, GL_RGBA16F});
  seedTexture  = RenderTexture2D({"u_seedTex" , winSize, 0, GL_TEXTURE_2D, GL_RG16F});
  jfaTexture   = RenderTexture2D({"u_jfaTex"  , winSize, 0, GL_TEXTURE_2D, GL_RG16F});
  sdfTexture   = RenderTexture2D({"u_sdfTex"  , winSize, 1, GL_TEXTURE_2D, GL_R16F});
  ping         = RenderTexture2D({"u_inputTex", winSize, 0, GL_TEXTURE_2D, GL_RGBA16F});
  pong         = RenderTexture2D({"u_inputTex", winSize, 0, GL_TEXTURE_2D, GL_RGBA16F});

  screenRect = Rectangle2D(winSize, winSize / 2u);

  seedShader.setUniformTexture(sceneTexture.texture);

  jfaShader.setUniformTexture(ping.texture); // same unit
  jfaShader.setUniform2f("u_resolution", winSize);

  sdfShader.setUniformTexture(jfaTexture.texture);

  giShader.setUniformTexture(sceneTexture.texture);
  giShader.setUniformTexture(sdfTexture.texture);
  giShader.setUniform2f("u_resolution", winSize);

  tex2DShader.setUniformTexture("u_texture", 0);

  calcPassesJFA();
}

void RenderConfig::onMousePressed(const vec2& pos) {
  isDrawing = true;
  mousePrevPos = pos;
  drawMouseAt(pos);
}

void RenderConfig::onMouseReleased() {
  isDrawing = false;
}

void RenderConfig::onMouseMoved(const vec2& pos) {
  if (!isDrawing) return;

  vec2 v = pos - mousePrevPos;
  float dist = v.length();
  if (dist < 0.1f) return;

  constexpr float step = 0.25f;
  constexpr int maxSteps = 100;
  int steps = std::min(static_cast<int>(dist  / step), maxSteps);

  for (int i = 0; i < steps; i++) {
    float t = static_cast<float>(i) / steps;
    drawMouseAt(mousePrevPos + t * (pos - mousePrevPos));
  }

  mousePrevPos = pos;
}

void RenderConfig::draw() {
  drawSeed();
  drawJFA();
  drawSDF();
  drawGI();
}

void RenderConfig::calcPassesJFA() {
  if (autoJfaPasses)
    jfaPasses = static_cast<int>(std::ceil(std::log2(std::max(winSize.x, winSize.y))));
}

void RenderConfig::drawMouseAt(const vec2& point) {
  ScopedProfileTask task("drawMouseAt");

  mouseShader.setUniform2f("u_pos", point);
  mouseShader.setUniform3f("u_color", mouseColor);
  mouseShader.setUniform1f("u_radius", mouseRadius);

  glEnable(GL_BLEND);
  sceneTexture.draw(screenRect, mouseShader);
}

void RenderConfig::drawSeed() {
  ScopedProfileTask task("drawSeed");

  seedTexture.clear();

  sceneTexture.texture.bind();
  seedTexture.draw(screenRect, seedShader);
  sceneTexture.texture.unbind();
}

void RenderConfig::drawJFA() {
  ScopedProfileTask task("drawJFA");

  RenderTexture2D* inputTex = &ping;
  RenderTexture2D* outputTex = &pong;
  RenderTexture2D* lastTex = &pong;

  inputTex->clear();
  outputTex->clear();

  // Fill input
  assert(seedTexture.texture.getUnit() == 0);
  seedTexture.texture.bind();
  inputTex->draw(screenRect, tex2DShader);
  seedTexture.texture.unbind();

  for (int i = 0; i < jfaPasses; i++) {
    jfaShader.setUniform1i("u_offset", 1 << (jfaPasses - i - 1));

    outputTex->clear();

    inputTex->texture.bind();
    outputTex->draw(screenRect, jfaShader);
    inputTex->texture.unbind();

    RenderTexture2D* temp = inputTex;
    inputTex = outputTex;
    outputTex = temp;
    lastTex = temp;
  }

  lastTex->texture.bind();
  jfaTexture.draw(screenRect, tex2DShader);
  lastTex->texture.unbind();
}

void RenderConfig::drawSDF() {
  ScopedProfileTask task("drawSDF");

  sdfTexture.clear();

  jfaTexture.texture.bind();
  sdfTexture.draw(screenRect, sdfShader);
  jfaTexture.texture.unbind();
}

void RenderConfig::drawGI() {
  ScopedProfileTask task("drawGI");

  RenderTexture2D* inputTex = &ping;
  RenderTexture2D* outputTex = &pong;
  RenderTexture2D* lastTex = &pong;

  inputTex->clear();
  outputTex->clear();

  assert(sceneTexture.texture.getUnit() == 0);
  sceneTexture.texture.bind();
  inputTex->draw(screenRect, tex2DShader);
  sceneTexture.texture.unbind();

  sdfTexture.texture.bind();
  giShader.setUniform1i("u_rayCountBase", rayCountBase);

  for (int i = 2; i >= 1; i--) {
    giShader.setUniform1i("u_rayCount", glm::pow(rayCountBase, i));
    giShader.setUniform1i("u_rayMaxSteps", rayMaxSteps);
    giShader.setUniform1f("u_epsilon", epsilon);
    giShader.setUniform1f("u_scale", scale);
    giShader.setUniform1f("u_srgb", srgb);

    outputTex->clear();

    inputTex->texture.bind();
    outputTex->draw(screenRect, giShader);
    inputTex->texture.unbind();

    RenderTexture2D* temp = inputTex;
    inputTex = outputTex;
    lastTex = outputTex;
    outputTex = temp;
  }

  sdfTexture.texture.unbind();

  FBO::unbind();
  glDisable(GL_BLEND);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  lastTex->texture.bind();
  screenRect.draw(finalShader);
  lastTex->texture.unbind();
}

