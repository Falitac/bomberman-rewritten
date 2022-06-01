#include "../../App.hpp"
#include "FreeCamera.hpp"

#include <glfw/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/core.h>

FreeCamera::FreeCamera(float fov, glm::vec3 position)
: fov(fov)
, pitch(0.f)
, yaw (-90.f)
, up(glm::vec3{0.f, 1.f, 0.f})
, sensitivity(0.05f)
{
  this->position = position;
}

void FreeCamera::handleInput(App& app) {
  Input& input = app.input;
  auto angleRotationSpeed = 10.3f;
  if(input.isKeyPressed(GLFW_KEY_SPACE)) {
    input.centerizeMouse(app);
    auto mouseOffset = input.getMouseOffset();
    fmt::print(fmt::fg(fmt::color::blanched_almond), "Offset: {}x{}\n", mouseOffset.x, mouseOffset.y);

    yaw += mouseOffset.x * sensitivity;
    pitch -= mouseOffset.y * sensitivity;
  }

  if(pitch > 90.f) {
    pitch = 90.f;
  }
  if(pitch < -90.f) {
    pitch = -90.f;
  }

  auto speed = 0.05f;
  if(input.isKeyPressed(GLFW_KEY_W)) {
    position += direction * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_S)) {
    position -= direction * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_A)) {
    position -= right * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_D)) {
    position += right * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_R)) {
    position += up * speed;
  }
  if(input.isKeyPressed(GLFW_KEY_F)) {
    position -= up * speed;
  }
  auto cosPitch = glm::cos(glm::radians(pitch));
  direction = glm::vec3{
    glm::cos(glm::radians(yaw)) * cosPitch,
    glm::sin(glm::radians(pitch)),
    glm::sin(glm::radians(yaw)) * cosPitch
  };
  direction = glm::normalize(direction);
  right = glm::normalize(glm::cross(direction, up));
}

glm::mat4 FreeCamera::getView() {
  return glm::lookAt(position, position + direction, up);
}

glm::mat4 FreeCamera::getProjection(float aspect) {
  return glm::perspective(glm::radians(45.f), aspect, 0.1f, 600.f);
}
