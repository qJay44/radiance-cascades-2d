#include "gui.hpp"

#include <algorithm>

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


  ImGui::Text("Cascade index: %d/%d", rc::config.drawCascadeIdx, rc::config.cascadeCount - 1);
  ImGui::SameLine();

  if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
    u8& idx = rc::config.drawCascadeIdx;
    if (idx) idx--;
  };
  ImGui::SameLine();

  if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
    u8& idx = rc::config.drawCascadeIdx;
    idx = std::min(u8(idx + 1), u8(rc::config.cascadeCount - 1));
  };


  ImGui::End();
}

