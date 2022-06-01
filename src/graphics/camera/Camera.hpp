#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Input.hpp"

class Camera {
public:
  virtual glm::mat4 getView() = 0;
  virtual glm::mat4 getProjection() = 0;

  virtual void handleInput(Input& input) = 0;

private:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
};

using CameraPtr = std::unique_ptr<Camera>;