#include "gui.hpp"

#include "imgui.h"
#include "../rc/Config.hpp"
#include "utils/utils.hpp"

static bool toggleWholeWindow = false;

RenderConfig* gui::renderConfig = nullptr;

void gui::toggle() {
  toggleWholeWindow = true;
}

void gui::draw() {
  static RunOnce a([]() {
    ImGui::SetNextWindowPos({0, 0});
  });

  ImGui::Begin("Debug");

  if (toggleWholeWindow) {
    ImGui::SetWindowCollapsed(!ImGui::IsWindowCollapsed());
    toggleWholeWindow = false;
  }

  if (ImGui::CollapsingHeader("Config")) {
    if (ImGui::SliderFloat("Cascade0 interval", &rc::config.interval0, 1.f, 30.f))
      rc::config.calcCascadeCount();

    ImGui::SliderFloat("Cascade0 spacing", &rc::config.linear0, 1.f, 30.f);

    ImGui::Text("Epsilon: %f", rc::config.epsilon); ImGui::SameLine();
    if (ImGui::ArrowButton("##left" , ImGuiDir_Left))  {rc::config.epsilon *= 0.1f;} ImGui::SameLine();
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {rc::config.epsilon *= 10.f;}

    if (renderConfig) {
      const uvec2& sdfSize = renderConfig->sdfTexture.getSize();
      const uvec2& sdfSizeNative = renderConfig->sdfTexture.getSizeNative();
      ImGui::Text("sdfTexture size: {%d, %d}, native: {%d, %d}", sdfSize.x, sdfSize.y, sdfSizeNative.x, sdfSizeNative.y);

      const uvec2& sceneSize = renderConfig->sceneTexture.getSize();
      const uvec2& sceneSizeNative = renderConfig->sceneTexture.getSizeNative();
      ImGui::Text("sceneTexture size: {%d, %d}, native: {%d, %d}", sceneSize.x, sceneSize.y, sceneSizeNative.x, sceneSizeNative.y);

      if (renderConfig->shapeContainer.mouseFollowShape) {
        const vec2& mouseCirclePos = renderConfig->shapeContainer.mouseFollowShape->getPosition();
        ImGui::Text("Mouse circle pos: {%.2f, %.2f}", mouseCirclePos.x, mouseCirclePos.y);
      }
    } else {
      ImGui::TextDisabled("[RenderConfig] isn't linked");
    }
  }

  if (ImGui::CollapsingHeader("Textures")) {
    static const GLint swizzle[4] = {GL_RED, GL_RED, GL_RED, GL_ONE};

    static float scale = 0.2f;
    ImGui::SliderFloat("Scaling", &scale, 0.1f, 1.f);

    ImGui::SeparatorText("Scene");
    const vec2& sceneSize = renderConfig->sceneTexture.getSize();
    ImGui::Image(renderConfig->sceneTexture.getId(), sceneSize * scale, {0.f, 1.f}, {1.f, 0.f});

    ImGui::SeparatorText("SDF");
    const vec2& sdfSize = renderConfig->sdfTexture.getSize();
    renderConfig->sdfTexture.bind();
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    ImGui::Image(renderConfig->sdfTexture.getId(), sdfSize * scale, {0.f, 1.f}, {1.f, 0.f});
  }

  ImGui::End();
}

