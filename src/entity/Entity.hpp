#pragma once

#include <glm/glm.hpp>
#include <memory>

class Entity {
public:
  virtual void update() = 0;
  virtual void render() = 0;

protected:
  float health;
  glm::vec3 position;
  glm::vec3 bounds;
};


using EntityPtr = std::unique_ptr<Entity>;