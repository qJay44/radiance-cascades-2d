#include "gui.hpp"

#include "imgui.h"
#include "../rc/Config.hpp"
#include "utils/utils.hpp"

static bool toggleWholeWindow = false;

ShapeContainer* gui::shapeContainer = nullptr;

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

  if (ImGui::SliderFloat("Cascade0 interval", &rc::config.interval0, 1.f, 30.f))
    rc::config.calcCascadeCount();

  ImGui::SliderFloat("Cascade0 spacing", &rc::config.linear0, 1.f, 30.f);

  ImGui::Text("Epsilon: %f", rc::config.epsilon); ImGui::SameLine();
  if (ImGui::ArrowButton("##left" , ImGuiDir_Left))  {rc::config.epsilon *= 0.1f;} ImGui::SameLine();
  if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {rc::config.epsilon *= 10.f;}

  if (!shapeContainer)
    error("[gui::draw] Didnt link ShapeContainer");

  ImGui::End();
}

