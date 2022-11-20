#include "AABB.hpp"

bool AABB::isCollidingAABB(AABB& other) {
  sort();
  other.sort();

  return
    collider.first.x <= other.collider.second.x &&
    collider.second.x >= other.collider.first.x &&
    collider.first.y <= other.collider.second.y &&
    collider.second.y >= other.collider.first.y &&
    collider.first.z <= other.collider.second.z &&
    collider.second.z >= other.collider.first.z ;
}

void AABB::sort() {
  if(collider.first.x > collider.second.x) {
    std::swap(collider.first.x, collider.second.x);
  }
  if(collider.first.y > collider.second.y) {
    std::swap(collider.first.y, collider.second.y);
  }
  if(collider.first.z > collider.second.z) {
    std::swap(collider.first.z, collider.second.z);
  }
}
