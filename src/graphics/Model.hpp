#pragma once

#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model {
public:
  Model();
  Model(Model& other) = delete;
  Model(Model&& other) = default;

  void loadModel(const std::string& path);
  void render(Shader& shader);

private:
  std::vector<Mesh> meshes;

  std::vector<Vertex> parseVertices(aiMesh*& mesh);
  std::vector<uint32_t> parseFaces(aiMesh*& mesh);
  Mesh parseMesh(aiMesh*& mesh);
};
