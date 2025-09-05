#include "RenderConfig.hpp"

#include <algorithm>
#include <cmath>

RenderConfig::RenderConfig(ProfilerManager* profilerManager) {
  this->profilerManager = profilerManager;
}

void RenderConfig::init(uvec2 winSize) {
  this->winSize = winSize;

  sceneTexture = RenderTexture2D({"u_sceneTex", winSize, 0, GL_TEXTURE_2D, GL_RGBA16F});
  seedTexture  = RenderTexture2D({"u_seedTex" , winSize, 0, GL_TEXTURE_2D, GL_RG16F});
  sdfTexture   = RenderTexture2D({"u_sdfTex"  , winSize, 1, GL_TEXTURE_2D, GL_RG16F});
  pingJFA      = RenderTexture2D({"u_inputTex", winSize, 0, GL_TEXTURE_2D, GL_RG16F});
  pongJFA      = RenderTexture2D({"u_inputTex", winSize, 0, GL_TEXTURE_2D, GL_R16F});

  screenRect = Rectangle2D(winSize, winSize / 2u);

  seedShader.setUniformTexture(sceneTexture.texture);
  seedShader.setUniform2f("u_resolution", winSize);

  jfaShader.setUniformTexture(pingJFA.texture); // same unit
  jfaShader.setUniform2f("u_resolution", winSize);

  sdfShader.setUniformTexture("u_jfaTex", pingJFA.texture.getUnit()); // same unit
  sdfShader.setUniform2f("u_resolution", winSize);

  giShader.setUniformTexture(sceneTexture.texture);
  giShader.setUniformTexture(sdfTexture.texture);
  giShader.setUniform2f("u_resolution", winSize);

  tex2DShader.setUniformTexture("u_texture", 0);

  calcPassesJFA();
}

void RenderConfig::addProfilier(ProfilerManager* pm) {
  profilerManager = pm;
}

void RenderConfig::onMousePressed(const vec2& pos) {
  isDrawing = true;
  mouse.prevPos = pos;
  drawMouseAt(pos);
}

void RenderConfig::onMouseReleased() {
  isDrawing = false;
}

void RenderConfig::onMouseMoved(const vec2& pos) {
  if (!isDrawing) return;

  vec2 v = pos - mouse.prevPos;
  float dist = v.length();
  if (dist < 0.1f) return;

  constexpr float step = 0.25f;
  constexpr int maxSteps = 100;
  int steps = std::min(static_cast<int>(dist  / step), maxSteps);

  for (int i = 0; i < steps; i++) {
    float t = static_cast<float>(i) / steps;
    drawMouseAt(mouse.prevPos + t * (pos - mouse.prevPos));
  }

  mouse.prevPos = pos;
}

void RenderConfig::update() {
  drawSeed();
  drawJFA();
  drawSDF();
}

void RenderConfig::clearScene() {
  sceneTexture.clear();
}

void RenderConfig::drawGI() {
  sceneTexture.texture.bind();
  sdfTexture.texture.bind();

  giShader.setUniform1i("u_stepsPerRay", stepsPerRay);
  giShader.setUniform1i("u_raysPerPixel", raysPerPixel);
  giShader.setUniform1f("u_epsilon", epsilon);

  screenRect.draw(giShader);

  sceneTexture.texture.unbind();
  sdfTexture.texture.unbind();
}

void RenderConfig::calcPassesJFA() {
  if (autoJfaPasses)
    jfaPasses = static_cast<int>(std::ceil(std::log2(std::max(winSize.x, winSize.y))));
}

void RenderConfig::drawMouseAt(const vec2& point) {
  profilerManager->startTask([&] {
    mouse.shader.setUniform2f("u_pos", point);
    mouse.shader.setUniform3f("u_color", mouse.drawColor);
    mouse.shader.setUniform1f("u_radius", mouse.drawRadius);

    sceneTexture.draw(screenRect, mouse.shader);
  }, "drawMouseAt");
}

void RenderConfig::drawSeed() {
  profilerManager->startTask([&] {
    seedTexture.clear();
    seedTexture.draw(screenRect, seedShader, &sceneTexture.texture);
  }, "drawSeed");
}

void RenderConfig::drawJFA() {
  profilerManager->startTask([&] {
    RenderTexture2D* inputTex = &pingJFA;
    RenderTexture2D* outputTex = &pongJFA;
    jfaTex = &pongJFA;

    inputTex->clear();
    inputTex->draw(screenRect, tex2DShader, &seedTexture.texture);

    for (int i = 0; i < jfaPasses; i++) {
      jfaShader.setUniform1i("u_offset", 1 << (jfaPasses - i - 1));

      outputTex->clear();
      outputTex->draw(screenRect, jfaShader, &inputTex->texture);

      RenderTexture2D* temp = inputTex;
      inputTex = outputTex;
      outputTex = temp;
      jfaTex = temp;
    }
  }, "drawJFA");
}

void RenderConfig::drawSDF() {
  profilerManager->startTask([&] {
    sdfTexture.clear();
    sdfTexture.draw(screenRect, sdfShader, &jfaTex->texture);
  }, "drawSDF");
}

