#pragma once

#include <utility>
#include <glm/glm.hpp>

class AABB {
public:
  AABB() = default;

  bool isCollidingAABB(AABB& other);
  bool isCollidingSphere(glm::vec3& position, float radius);
  void sort();

  void setFirstCorner(const glm::vec3& corner) { collider.first = corner; };
  void setSecondCorner(const glm::vec3& corner) { collider.second = corner; };

private:
  std::pair<glm::vec3, glm::vec3> collider;
};
