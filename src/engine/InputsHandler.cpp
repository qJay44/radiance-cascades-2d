#include "InputsHandler.hpp"

#include "gui.hpp"

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
  }
}

void InputsHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

