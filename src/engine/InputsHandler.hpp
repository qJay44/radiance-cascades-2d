#pragma once

struct InputsHandler {
  InputsHandler() = delete;

  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

