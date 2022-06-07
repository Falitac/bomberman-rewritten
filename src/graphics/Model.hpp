#pragma once

#include "Mesh.hpp"

class Model {
public:
  Model();

  void loadModel(const std::string& path);

  void render(Shader& shader);

private:
  std::vector<Mesh> meshes;

};
