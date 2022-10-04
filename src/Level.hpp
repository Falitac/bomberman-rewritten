#pragma once

#include "entity/Player.hpp"
#include "entity/Bomb.hpp"
#include "graphics/Mesh.hpp"
#include "App.hpp"

#include <vector>
#include <list>


class Level {
public:
  using BoardData = std::vector<std::vector<int>>;
  Level(uint32_t cols, uint32_t rows, float boxSize = 2.f);
  ~Level();

  void update();
  void render();

private:
  uint32_t rows;
  uint32_t cols;

  float width;
  float height;

  Skybox skybox;
  Mesh mesh;
  Model model;

  Player player;
  BoardData board;
  Mesh boardPlane;
  Model bombModel;
  Model playerModel;
  std::vector<Bomb> bombs;

  friend class Player;
};
