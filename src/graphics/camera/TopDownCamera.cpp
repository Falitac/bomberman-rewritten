#include "../../App.hpp"
#include "TopDownCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

TopDownCamera::TopDownCamera(glm::vec3& pointToLookAt, float fov, float height)
: Camera({}, fov)
, pointToLookAt(pointToLookAt)
, height(height)
, velocity(0.f)
, cameraFollowCoefficient(0.93f)
, followAngle(0.f)
{
}

void TopDownCamera::handleInput(App& app) {
  auto radius = 3.f;

  auto offset = glm::vec3{
    0.f,
    height,
    0.f
    };

  auto pointDiff = position - pointToLookAt;
  position = pointToLookAt + pointDiff * cameraFollowCoefficient;
  followAngle = followAngle - glm::dot(glm::normalize(pointDiff), offset - glm::vec3{0.f, height, 0.f});

  view = glm::lookAt(offset + position, position + (pointToLookAt - position), glm::vec3{0.f, 1.0f, 0.f});
  projection = glm::perspective(glm::radians(fov), aspect, nearView, farView);
}
