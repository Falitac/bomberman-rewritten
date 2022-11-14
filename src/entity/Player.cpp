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
  collision(level);
  
  model = glm::translate(glm::mat4{1.0f}, position + levitationPos);
  _App->setCameraPoint(position);
}

void Player::collision(Level& level) {
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
  aabb.setFirstCorner(position - glm::vec3{0.5});
  aabb.setSecondCorner(position + glm::vec3{0.5});

  int counter = 0;
  for(int y = 0; y < level.getRowCount(); y++) {
    for(int x = 0; x < level.getColCount(); x++) {
      auto block = level.getBlock(x, y);
      if(block == Level::BlockType::None) {
        continue;
      }
      auto blockOffset = glm::vec3{x * level.getBlockSize(), 0.f, y * level.getBlockSize()};
      AABB blockAABB;
      blockAABB.setFirstCorner(blockOffset - glm::vec3{level.getBlockSize() * .5f});
      blockAABB.setSecondCorner(blockOffset + glm::vec3{level.getBlockSize() * .5f});
      if(blockAABB.isCollidingAABB(aabb)) {
        //fmt::print("Block collision at [{}, {}]\n", x, y);
        counter++;
      };
    }
  }
  if(counter != 0)
    fmt::print("Collision count: {}\n", counter);

}

void Player::render() {

}

