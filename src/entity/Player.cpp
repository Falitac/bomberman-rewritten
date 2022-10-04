#include "Player.hpp"

#include <glm/ext.hpp>
#include "../App.hpp"

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
  glm::vec3 levitationPos{
    0.0f,
    0.2f * (sin(_App->getTimeSinceStart()) + 1.f),
    0.f
  };

  if(position.x < 0.0f) {
    position.x = 0.f;
  }
  if(position.x > level.width) {
    position.x = 0.f;
  }
  
  model = glm::translate(glm::mat4{1.0f}, position + levitationPos);
  _App->setCameraPoint(position);
}

void Player::render() {

}

