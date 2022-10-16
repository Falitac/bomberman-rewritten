#pragma once
#include "Camera.hpp"

class TopDownCamera : public Camera {
public:
  TopDownCamera(glm::vec3& pointToLookAt, float fov = 45., float height = 10.f);

  virtual void handleInput(App& app);

protected:
  glm::vec3& pointToLookAt;
  glm::vec3 velocity;
  glm::vec3 previousPosition;
  float cameraFollowCoefficient;
  float followAngle;
  float height;

};
