#pragma once

#include "Entity.hpp"


class Player : public Entity {
public:
  Player();

  virtual void update();
  virtual void render();

private:

};
