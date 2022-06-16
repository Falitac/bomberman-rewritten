#include "App.hpp"
#include "Input.hpp"

const std::map<int, bool>& Input::getKeys() const {
  return pressedKeys;
}

bool Input::isKeyPressed(int key) {
  return pressedKeys[key];
}

bool Input::isMouseButtonPressed(MButton button) {
  switch (button) {
  case MButton::Left:
    return mouse.left;
  case MButton::Right:
    return mouse.left;
  case MButton::Middle:
    return mouse.middle;
  default:
    return 0;
  }
}

void Input::setKeyPressed(int key, int value) {
  pressedKeys[key] = value;
  if(pressedKeys[key] && keyStateChange.contains(key)) {
    keyStateChange[key] = true;
  }
}

void Input::setMouseButtonPressed(int key, int value) {
  if(key == GLFW_MOUSE_BUTTON_1) {
    mouse.left = value;
  }
  if(key == GLFW_MOUSE_BUTTON_2) {
    mouse.right = value;
  }
  if(key == GLFW_MOUSE_BUTTON_3) {
    mouse.middle = value;
  }
}

void Input::setMousePosition(double xpos, double ypos) {
  mouse.xpos = xpos;
  mouse.ypos = ypos;
}

// To repair, doesn't work on touchpad
glm::vec2 Input::centerizeMouse(App& app) {
  static int ticker;
  double xpos, ypos;
  glfwGetCursorPos(app.window, &xpos, &ypos);
  glfwSetCursorPos(app.window, app.getWidth() / 2, app.getHeight() / 2);
  return {xpos - app.getWidth() / 2, ypos - app.getHeight() / 2};
}

bool Input::checkSinglePress(int key) {
  if(keyStateChange[key]) {
    keyStateChange[key] = false;
    return true;
  }

  return false;
}