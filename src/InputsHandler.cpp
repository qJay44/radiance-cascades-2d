#include "InputsHandler.hpp"

#include <cassert>

#include "GLFW/glfw3.h"
#include "gui.hpp"

RenderConfig* InputsHandler::renderConfig = nullptr;

// Single press or release
void InputsHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  assert(renderConfig);

  switch (key) {
    case GLFW_KEY_Q:
      if (action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_E:
      if (action == GLFW_PRESS)
        gui::toggle();
      break;
    case GLFW_KEY_R:
      if (action == GLFW_PRESS)
        renderConfig->clearScene();
      break;
  }
}

// Single click
// void InputsHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {}

// void InputsHandler::cursorMoveCallback(GLFWwindow* window, double x, double y) {}

void InputsHandler::process() {
  assert(renderConfig);

  dvec2 mouse;
  glfwGetCursorPos(global::window, &mouse.x, &mouse.y);

  renderConfig->onMouseMoved(mouse);

  if (glfwGetMouseButton(global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !gui::isHovered())
    renderConfig->onMousePressed(mouse);
  else
    renderConfig->onMouseReleased();
}

