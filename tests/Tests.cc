#include <gtest/gtest.h>

#include "../src/AABB.hpp"

TEST(point_inside, randomPoint) {
  AABB a, b;

  a.setFirstCorner(glm::vec3 {-5.91, -3.81, 0.});
  a.setSecondCorner(glm::vec3{ 0.21, -1.17, 3.});
  b.setFirstCorner(glm::vec3 {-3.00, -3.00, 1.});
  b.setSecondCorner(glm::vec3{ 5.00,  5.00, 5.});
  EXPECT_EQ(true, a.isCollidingAABB(b));
}

TEST(point_outside, randomPoint) {
  AABB a, b;

  a.setFirstCorner(glm::vec3 {-5.91, -3.81, 0.});
  a.setSecondCorner(glm::vec3{-3.96556,-1.68095,1.2093});
  b.setFirstCorner(glm::vec3 {-3.00, -3.00, 1.});
  b.setSecondCorner(glm::vec3{ 5.00,  5.00, 5.});
  EXPECT_EQ(false, a.isCollidingAABB(b));

}

TEST(point_inside, swapped) {
  AABB a, b;

  a.setFirstCorner(glm::vec3 {-5.91, -3.81, 0.});
  a.setSecondCorner(glm::vec3{-3.96556,-1.68095,1.2093});
  b.setFirstCorner(glm::vec3 {-3.00, -3.00, 1.});
  b.setSecondCorner(glm::vec3{ 5.00,  5.00, 5.});
  EXPECT_EQ(false, b.isCollidingAABB(a));

}

TEST(point_outside, swapped) {
  AABB a, b;

  a.setFirstCorner(glm::vec3 {-5.91, -3.81, 0.});
  a.setSecondCorner(glm::vec3{-3.96556,-1.68095,1.2093});
  b.setFirstCorner(glm::vec3 {-3.00, -3.00, 1.});
  b.setSecondCorner(glm::vec3{ 5.00,  5.00, 5.});
  EXPECT_EQ(false, b.isCollidingAABB(a));

}