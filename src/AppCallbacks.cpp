#include "App.hpp"

void App::initializeCallbacks() {
  glfwSetWindowUserPointer(window, _App);

  auto errorCallback = [](int error_code, const char* description) {
    fmt::print(fmt::fg(fmt::color::red), "GLFW error: {}\n", description);
  };
  auto mouseCallback = [](GLFWwindow* window, int button, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->mouseCallback(window, button, action, mods);
  };
  auto cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->cursorPosCallback(window, xpos, ypos);
  };
  auto keyboardCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->keyboardCallback(window, key, scancode, action, mods);
  };
  auto windowSizeCallback = [](GLFWwindow* window, int width, int height) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->windowSizeCallback(window, width, height);
  };

  glfwSetErrorCallback(errorCallback);
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorPosCallback);
  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void App::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
  _App->input.setMouseButtonPressed(button, action);
}

void App::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  _App->input.setMousePosition(xpos, ypos);
}

void App::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    _App->input.setKeyPressed(key, true);
  }
  if(action == GLFW_RELEASE) {
    _App->input.setKeyPressed(key, false);
  }
}

void App::windowSizeCallback(GLFWwindow* window, int width, int height) {
  static int printCounter;
  if(printCounter > 20) {
    printCounter = 0;
    fmt::print(fmt::fg(fmt::color::aquamarine),
      "Window new dimensions {}x{}\n", width, height);
  }
  printCounter++;

  glViewport(0, 0, width, height);
  this->width = width;
  this->height = height;
}