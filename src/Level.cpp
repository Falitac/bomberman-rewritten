#include "Level.hpp"

#include <fmt/core.h>

Level::Level(uint32_t rows, uint32_t cols, float boxSize)
: rows(rows)
, cols(cols) {
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

  height = rows * boxSize;
  width = cols * boxSize;
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

  shader.passUniform("Model", glm::mat4{1.f});
  cube.render(shader);

  auto& skyboxShader = assets.getShader("skybox");
  skyboxShader.use();
  auto viewportDownscaled = glm::mat4{glm::mat3{camera.getView()}};
  skyboxShader.passUniform("View", viewportDownscaled);
  skyboxShader.passUniform("Projection", camera.getProjection());

  skybox.render(skyboxShader, assets.getCubemap("skybox-lightblue"));
}