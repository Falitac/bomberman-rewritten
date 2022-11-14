#pragma once
#include "Camera.hpp"

#include <vector>
#include <list>

class FreeCamera : public Camera {
public:
  FreeCamera(glm::vec3 position = {}, float fov = 45., float sensititivity = 0.1f, float speed = 4.f);

  virtual void handleInput();

  inline void setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
  }

protected:
  float roll;
  float pitch;
  float yaw;

  float sensitivity;
  float speed;

  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 right;

  std::vector<glm::vec2> offsetHistory;
  size_t maxOffsetHistory;
  void calculateAngleChanges(glm::vec2& mouseOffset);
  void calculatePosition(Input& input, float dt);
  void calculateVectors();
};
