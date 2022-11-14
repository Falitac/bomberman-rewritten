#pragma once

#include "entity/Player.hpp"
#include "entity/Bomb.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Skybox.hpp"

#include <vector>
#include <list>


class Level {
public:
  enum class BlockType {
    None,
    Breakable,
    Solid,
  };
  using BoardData = std::vector<BlockType>;
  Level(uint32_t cols, uint32_t rows, float boxSize = 2.f, const std::string& levelFileName = "board0.txt");
  ~Level();

  void update();
  void render();

  BlockType& getBlock(size_t x, size_t y) { return board[y * rows + x]; }
  auto getBlockSize() { return boxSize; }
  auto getRowCount() { return rows; }
  auto getColCount() { return cols; }

private:
  uint32_t rows;
  uint32_t cols;
  float boxSize;

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
  Model cube;
  std::vector<Bomb> bombs;

  friend class Player;

private:
  void spawnBomb(Bomb&& bomb);
  bool loadBoardFromFile(const std::string& levelFileName);
  void renderBoard(Shader& shader);
  void renderBombs(Shader& shader);
};
