#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Input.hpp"

class App;

class Camera {
public:
  Camera(glm::vec3 position, float fov = 45.f) 
  : fov(fov)
  , position(position) {
  }
  virtual ~Camera()
  { }

  virtual void handleInput(App& app) = 0;

  glm::vec3& getPosition() {
    return position;
  }
  glm::mat4& getView() {
    return view;
  }
  glm::mat4& getProjection() {
    return projection;
  }

protected:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  float fov;
  float aspect;
};

using CameraPtr = std::unique_ptr<Camera>;