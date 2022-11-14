#include "../../App.hpp"
#include "FreeCamera.hpp"

#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/core.h>

FreeCamera::FreeCamera(glm::vec3 position, float fov, float sensititivity, float speed)
: Camera(position, fov)
, pitch(0.f)
, yaw (-90.f)
, up(glm::vec3{0.f, 1.f, 0.f})
, sensitivity(sensitivity)
, speed(speed)
, maxOffsetHistory(16)
{
  assert(maxOffsetHistory < 32);
}

void FreeCamera::update() {
  float dt = _App->getDt();
  Input& input = _App->input;

  if(input.isKeyPressed(GLFW_KEY_SPACE)) {
    _App->hideCursor();
    auto mouseOffset = input.centerizeMouse(*_App);
    calculateAngleChanges(mouseOffset);
  } else {
    _App->showCursor();
  }
  calculatePosition(input, dt);
  calculateVectors();
}

void FreeCamera::calculateAngleChanges(glm::vec2& mouseOffset) {
  if(offsetHistory.size() < maxOffsetHistory) { 
    offsetHistory.push_back(mouseOffset);
  } else {
    offsetHistory.back() = mouseOffset;
    glm::vec2 avg = {0.0, 0.0};
    float totalDenominator = 0.0f;
    float weightChange = 2.f;
    for(size_t i = 0; i < offsetHistory.size(); i++) {
      float weight = (1 << i) * weightChange;
      avg += offsetHistory[i] * weight;
      totalDenominator += weight;
    }
    mouseOffset = avg / totalDenominator;
    std::rotate(offsetHistory.begin(), offsetHistory.begin() + 1, offsetHistory.end());
  }

  yaw += mouseOffset.x * sensitivity;
  pitch -= mouseOffset.y * sensitivity;

  if(pitch > 89.f) {
    pitch = 89.f;
  }
  if(pitch < -89.f) {
    pitch = -89.f;
  }
}

void FreeCamera::calculatePosition(Input& input, float dt) {
  auto positionChange = glm::vec3{0.f};
  if(input.isKeyPressed(GLFW_KEY_W)) {
    positionChange += direction * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_S)) {
    positionChange -= direction * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_A)) {
    positionChange -= right * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_D)) {
    positionChange += right * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_R)) {
    positionChange += up * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_F)) {
    positionChange -= up * speed;
  }
  position += dt * positionChange;
}

void FreeCamera::calculateVectors() {
  auto cosPitch = glm::cos(glm::radians(pitch));
  direction = glm::vec3{
    glm::cos(glm::radians(yaw)) * cosPitch,
    glm::sin(glm::radians(pitch)),
    glm::sin(glm::radians(yaw)) * cosPitch
  };
  right = glm::normalize(glm::cross(direction, up));

  view = glm::lookAt(position, position + direction, up);
  projection = glm::perspective(glm::radians(fov), aspect, nearView, farView);
}
