#include "Player.hpp"

#include <glm/ext.hpp>
#include "../App.hpp"
#include "../Level.hpp"

Player::Player() {
  position = glm::vec3{0.0f};
  speed = 10.f;
}

void Player::update(Level& level) {
  auto& input = _App->input;
  if(input.isKeyPressed(GLFW_KEY_A)) {
    position.x += speed * _App->getDt();
  }
  if(input.isKeyPressed(GLFW_KEY_D)) {
    position.x += -speed * _App->getDt();
  }
  if(input.isKeyPressed(GLFW_KEY_W)) {
    position.z += speed * _App->getDt();
  }
  if(input.isKeyPressed(GLFW_KEY_S)) {
    position.z += -speed * _App->getDt();
  }
  glm::vec3 levitationPos{
    0.0f,
    0.2f * (sin(_App->getTimeSinceStart()) + 1.f) + 0.2f,
    0.f
  };

  if(position.x < 0.0f) {
    position.x = 0.f;
  }
  if(position.x > level.width) {
    position.x = level.width;
  }
  if(position.z < 0.0f) {
    position.z = 0.f;
  }
  if(position.z > level.height) {
    position.z = level.height;
  }
  
  model = glm::translate(glm::mat4{1.0f}, position + levitationPos);
  _App->setCameraPoint(position);
}

void Player::render() {

}

