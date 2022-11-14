#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../AABB.hpp"

class Level;

class Entity {
public:
  virtual void update(Level& level) { };
  virtual void render() { };

protected:
  float health;
  glm::vec3 position;
  AABB aabb;
};


using EntityPtr = std::unique_ptr<Entity>;