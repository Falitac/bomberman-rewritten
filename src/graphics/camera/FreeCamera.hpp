#pragma once

#include "Camera.hpp"

class FreeCamera :public Camera {
public:
  FreeCamera(float fov = 45., glm::vec3 position = {});

  virtual glm::mat4 getView();
  virtual glm::mat4 getProjection(float aspect);
  virtual void handleInput(App& app);

  inline void setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
  }

protected:
  float fov;
  float roll;
  float pitch;
  float yaw;

  float sensitivity;

  glm::vec3 direction;
  glm::vec3 up;
  glm::vec3 right;
};
