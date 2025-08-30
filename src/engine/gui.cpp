#include "gui.hpp"

#include "imgui.h"
#include "../rc/Config.hpp"

static bool toggleWholeWindow = false;

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

  ImGui::SliderFloat("Cascade0 interval", &rc::config.interval0, 1.f, 30.f);

  static int stepsPerRay = rc::config.stepsPerRay;
  if (ImGui::SliderInt("Steps per ray", &stepsPerRay, 1, 256)) {
    rc::config.stepsPerRay = static_cast<u8>(stepsPerRay);
  }

  ImGui::Text("Epsilon: %f", rc::config.epsilon);
  ImGui::SameLine();

  if (ImGui::ArrowButton("##left" , ImGuiDir_Left))  {rc::config.epsilon *= 0.1f;} ImGui::SameLine();
  if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {rc::config.epsilon *= 10.f;} ImGui::SameLine();

  ImGui::End();
}

