#include "RenderConfig.hpp"
#include "gui.hpp"

#include <cassert>

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/utils.hpp"

static bool toggleWholeWindow = false;

RenderConfig* gui::renderConfig = nullptr;

bool gui::isHovered() {
  return ImGui::GetIO().WantCaptureMouse;
}

void gui::toggle() {
  toggleWholeWindow = true;
}

void setCursorUnderCurrentWidget() {
  ImVec2 imagePos = ImGui::GetItemRectMin();
  ImVec2 imageSize = ImGui::GetItemRectSize();

  float widgetPosX = imagePos.x;
  float widgetPosY = imagePos.y + imageSize.y + ImGui::GetStyle().ItemInnerSpacing.y;
  ImGui::SetCursorPos(ImVec2(widgetPosX, widgetPosY));
}

void gui::draw() {
  static RunOnce a([]() {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float);
  });

  assert(renderConfig != nullptr);

  ImGui::Begin("Debug");

  if (toggleWholeWindow) {
    ImGui::SetWindowCollapsed(!ImGui::IsWindowCollapsed());
    toggleWholeWindow = false;
  }

  if (ImGui::CollapsingHeader("Draw")) {
    ImGui::SliderFloat("Radius", &renderConfig->mouseRadius, 1.f, 100.f);
    ImGui::ColorEdit3("Color", glm::value_ptr(renderConfig->mouseColor));

    if (ImGui::Button("Clear"))
      renderConfig->clearScene();
  }

  if (ImGui::CollapsingHeader("Global Illumination")) {
    ImGui::SliderInt("Base ray count", &renderConfig->rayCountBase, 4, 16);
    ImGui::SliderInt("Max steps per ray", &renderConfig->rayMaxSteps, 1, 32);
    ImGui::SliderFloat("interval0", &renderConfig->interval0, 0.01f, 1.41f);
    ImGui::SliderFloat("Scale", &renderConfig->scale, 0.01f, 1.f);
    ImGui::SliderFloat("sRGB", &renderConfig->srgb, 1.f, 20.f);
  }

  if (ImGui::CollapsingHeader("Textures")) {
    static float scale = 0.2f;
    ImGui::SliderFloat("Size", &scale, 0.1f, 1.f);

    float maxWidth = renderConfig->winSize.x * 0.3f;

    if (ImGui::TreeNode("Scene")) {
      // ----- The image ------------------------------------------- //

      vec2 sceneSize = renderConfig->sceneTexture.texture.getSize();
      ImGui::Image(renderConfig->sceneTexture.texture.getId(), sceneSize * scale);

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Seed")) {
      const Texture2D& seedTex = renderConfig->seedTexture.texture;
      vec2 seedSize = seedTex.getSize();
      ImGui::Image(seedTex.getId(), seedSize * scale);

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("JFA")) {
      // ----- The image ------------------------------------------- //

      const Texture2D& jfaTex = renderConfig->jfaTexture.texture;
      vec2 jfaSize = vec2(jfaTex.getSize());
      ImGui::Image(jfaTex.getId(), jfaSize * scale);

      // ----- The config ------------------------------------------ //

      ImGui::SameLine();
      ImGui::BeginGroup();
      ImGui::PushItemWidth(maxWidth);

      if (ImGui::Checkbox("Auto calculate passes", &renderConfig->autoJfaPasses))
        renderConfig->calcPassesJFA();

      ImGui::BeginDisabled(renderConfig->autoJfaPasses);
      ImGui::SliderInt("Passes", &renderConfig->jfaPasses, 1, 20);
      ImGui::EndDisabled();

      ImGui::PopItemWidth();
      ImGui::EndGroup();

      // ----------------------------------------------------------- //

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("SDF")) {
      static const GLint swizzle[4] = {GL_RED, GL_RED, GL_RED, GL_ONE};

      const Texture2D& sdfTex = renderConfig->sdfTexture.texture;
      vec2 sdfSize = vec2(sdfTex.getSize());
      sdfTex.bind();
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
      ImGui::Image(sdfTex.getId(), sdfSize * scale);
      sdfTex.unbind();

      ImGui::TreePop();
    }
  }

  if (ImGui::CollapsingHeader("Profiler")) {
    assert(renderConfig->profilerManager != nullptr);
    renderConfig->profilerManager->render(400, 100, 200, 10);
  }

  ImGui::End();
}

