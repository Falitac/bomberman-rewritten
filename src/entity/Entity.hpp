#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Level;

class Entity {
public:
  virtual void update(Level& level) { };
  virtual void render() { };

protected:
  float health;
  glm::vec3 position;
  glm::vec3 bounds;

};


using EntityPtr = std::unique_ptr<Entity>;