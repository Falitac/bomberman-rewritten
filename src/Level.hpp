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
  int width;
  int height;

  Skybox skybox;
  Mesh mesh;
  Model model;

  Player player;
  BoardData board;
  Mesh boardPlane;
  Model playerModel;
  Model bombModel;
  std::vector<Bomb> bombs;
};
