#include "AABB.hpp"

bool AABB::isCollidingAABB(AABB& other) {
  sort();
  other.sort();

  if(collider.second.x >= other.collider.first.x &&
     collider.second.y >= other.collider.first.y  &&
     collider.second.z >= other.collider.first.z 
  ) {
    if(collider.second.x > other.collider.first.x) return false;
    if(collider.second.y > other.collider.first.y) return false;
    if(collider.second.z > other.collider.first.z) return false;
  } else {
    if(collider.second.x < other.collider.first.x) return false;
    if(collider.second.y < other.collider.first.y) return false;
    if(collider.second.z < other.collider.first.z) return false;
  }

  return true;
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
