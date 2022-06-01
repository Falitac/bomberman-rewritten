#include "Input.hpp"

const std::map<int, bool>& Input::getKeys() const {
  return pressedKeys;
}

void Input::setKeyPressed(int key, int value) {
  pressedKeys[key] = value;
  if(pressedKeys[key] && keyStateChange.contains(key)) {
    keyStateChange[key] = true;
  }
}

bool Input::checkSinglePress(int key) {
  if(keyStateChange[key]) {
    keyStateChange[key] = false;
    return true;
  }

  return false;
}