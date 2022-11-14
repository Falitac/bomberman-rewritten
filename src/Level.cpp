#include "Level.hpp"
#include "App.hpp"

#include <fmt/core.h>
#include <fmt/color.h>
#include <glm/gtc/matrix_transform.hpp>

Level::Level(uint32_t rows, uint32_t cols, float boxSize, const std::string& levelFileName)
: rows(rows)
, cols(cols)
, boxSize(boxSize)
, board(rows * cols, BlockType::None) {
  if(!loadBoardFromFile(levelFileName)) {
    fmt::print("Something went wrong with this board\n");
  }
  std::vector<Vertex> vertices = {
    {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
  };
  std::vector<GLuint> indices = {
    0, 1, 2,
    0, 2, 3
  };

  height = this->rows * boxSize;
  width = this->cols * boxSize;
  vertices[1].pos.x *= width;
  vertices[2].pos.z *= height;
  vertices[2].pos.x *= width;
  vertices[3].pos.z *= height;
  boardPlane.loadData(vertices, indices, {});

  bombModel.loadModel("assets/objects/bomb.obj");
  playerModel.loadModel("assets/objects/robo-boodie.obj");
  cube.loadModel("assets/objects/cube.obj");

  skybox.create();


}

Level::~Level() {
  boardPlane.destroy();
  skybox.destroy();
}

void Level::update() {
  player.update(*this);
}

void Level::render() {
  auto& assets = _App->assets;
  auto& camera = *_App->getCamera();
  auto position = glm::mat4{1.0f};

  auto& shader = assets.getShader("basic2");
  shader.use();
  shader.passMVP(position, camera.getView(), camera.getProjection());
  shader.passUniform("CameraPos", camera.getPosition());
  boardPlane.render(shader);

  shader.passUniform("Model", player.getModelMat4());
  playerModel.render(shader);

  renderBoard(shader);

  auto& skyboxShader = assets.getShader("skybox");
  skyboxShader.use();
  auto viewportDownscaled = glm::mat4{glm::mat3{camera.getView()}};
  skyboxShader.passUniform("View", viewportDownscaled);
  skyboxShader.passUniform("Projection", camera.getProjection());

  skybox.render(skyboxShader, assets.getCubemap("skybox-lightblue"));
}

bool Level::loadBoardFromFile(const std::string& levelFileName) {
  std::fstream fileInput("assets/boards/" + levelFileName);
  if(!fileInput.good()) {
    fmt::print(fmt::fg(fmt::color::red), "Couldn not load this file: {}\n", levelFileName);
    return false;
  }
  int rows, cols;
  fileInput >> rows >> cols;

  if(rows < 0 || cols < 0) {
    return false;
  }
  auto tmpBoard = BoardData(rows * cols, BlockType::None);

  for(int y = 0; y < rows; y++) {
    for(int x = 0; x < cols; x++) {
      int data;
      fileInput >> data;
      tmpBoard[y * cols + x] = static_cast<BlockType>(data);
    }
  }
  if(fileInput.bad()) {
    return false;
  }
  this->rows = rows;
  this->cols = rows;
  board = tmpBoard;

  return true;
}

void Level::renderBoard(Shader& shader) {
  const auto boxOffset = glm::vec3{boxSize / 2.f, 0.f, boxSize / 2.f};
  for(int y = 0; y < cols; y++) {
    for(int x = 0; x < rows; x++) {
      if(board[y * rows + x] == BlockType::None) {
        continue;
      }
      auto model = glm::mat4{1.f};
      auto translationVec = glm::vec3{boxSize * x, 0.f, boxSize * y} + boxOffset;
      model = glm::translate(model, translationVec);
      shader.passUniform("Model", model);
      cube.render(shader);
    }
  }
}

void Level::renderBombs(Shader& shader) {
  
}

void Level::spawnBomb(Bomb&& bomb) {
  bombs.push_back(std::move(bomb));
}