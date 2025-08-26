#include "gui.hpp"

#include "imgui.h"

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

  static float s = 3.f;
  ImGui::SliderFloat("Test slider", &s, 0.f, 10.f);

  ImGui::End();
}

