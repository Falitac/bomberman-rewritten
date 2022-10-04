#pragma once

#include "Entity.hpp"
#include "../graphics/Model.hpp"


class Player : public Entity {
public:
  Player();

  virtual void update(Level& level);
  virtual void render();

  glm::mat4 getModelMat4() {
    return model;
  }

  void setSpeed(float newSpeed) {
    if(newSpeed < 0.f) {
      return;
    }
    speed = newSpeed;
  }
  float getSpeed() { return speed; };
private:
  glm::mat4 model;
  float speed;
};
