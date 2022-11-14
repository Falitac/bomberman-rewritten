#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../Input.hpp"

class App;

class Camera {
public:
  Camera(glm::vec3 position, float fov = 45.f, float near = 0.05f, float far = 1024.f) 
  : fov(fov)
  , position(position)
  , nearView(near)
  , farView(far)
  {
  }
  virtual ~Camera() { }

  virtual void handleInput() = 0;

  glm::vec3& getPosition() {
    return position;
  }
  glm::mat4& getView() {
    return view;
  }
  glm::mat4& getProjection() {
    return projection;
  }

  void setFov(float fov) {
    if(fov <= 0.f) {
      return;
    }
    this->fov = fov;
  }

  void setAspect(float aspect) {
    if(aspect <= 0.f) {
      return;
    }
    this->aspect = aspect;
  }

  void setNearView(float nearView) {
    if(nearView <= 0.f) {
      return;
    }
    this->nearView = nearView;
  } 

  void setfarView(float farView) {
    if(farView <= 0.f) {
      return;
    }
    this->farView = farView;
  }

protected:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  float fov;
  float aspect;
  float nearView;
  float farView;
};

using CameraPtr = std::unique_ptr<Camera>;