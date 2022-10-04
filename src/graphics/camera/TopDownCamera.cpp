#include "../../App.hpp"
#include "TopDownCamera.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

TopDownCamera::TopDownCamera(glm::vec3& pointToLookAt, float fov, float height)
: Camera({}, fov)
, pointToLookAt(pointToLookAt)
, height(height)
{

}

void TopDownCamera::handleInput(App& app) {
  aspect = app.getAspect();
  position = pointToLookAt + glm::vec3{0.0f, height, -1.0f};
  view = glm::lookAt(position, pointToLookAt, glm::vec3{0.f, 1.0f, 0.f});
  projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 600.f);
}
