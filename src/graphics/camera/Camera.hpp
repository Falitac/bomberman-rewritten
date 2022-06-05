#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Input.hpp"

class App;

class Camera {
public:
  // @TODO change to returning by reference
  virtual glm::mat4 getView() = 0;
  virtual glm::mat4 getProjection() = 0;
  inline glm::vec3& getPosition() {
    return position;
  }

  virtual void handleInput(App& app) = 0;

protected:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  float aspect;
};

using CameraPtr = std::unique_ptr<Camera>;