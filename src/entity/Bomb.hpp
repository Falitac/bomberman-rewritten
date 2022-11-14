#pragma once

#include "Entity.hpp"

class Bomb : public Entity {
public:
  Bomb() = default;
  Bomb(Bomb&&) = default;

  virtual void update(Level& level);
  virtual void render();

private:

};
