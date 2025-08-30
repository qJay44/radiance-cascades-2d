#include "InputsHandler.hpp"

#include "GLFW/glfw3.h"
#include "gui.hpp"
#include "../rc/Config.hpp"

void InputsHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  switch (key) {
    case GLFW_KEY_Q:
      if (action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_E:
      if (action == GLFW_PRESS)
        gui::toggle();
      break;
    case GLFW_KEY_A:
      if (action == GLFW_PRESS) {
        u8& idx = rc::config.drawCascadeIdx;
        if (idx) idx--;
      }
      break;
    case GLFW_KEY_D:
      if (action == GLFW_PRESS) {
        u8& idx = rc::config.drawCascadeIdx;
        idx = std::min(u8(idx + 1), u8(rc::config.cascadeCount - 1));
      }
      break;
  }
}

void InputsHandler::process(ShapeContainer& shapeContainer) {
  dvec2 mouse;
  glfwGetCursorPos(global::window, &mouse.x, &mouse.y);

  if (glfwGetMouseButton(global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    shapeContainer.moveShape(mouse, true);
  } else {
    shapeContainer.moveShape(mouse, false);
  }
}

