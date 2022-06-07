#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fmt/core.h>
#include <fmt/color.h>

Model::Model() {

}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  const auto scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "Couldn't load the scene {}\n", path);
    return;
  }

  
}