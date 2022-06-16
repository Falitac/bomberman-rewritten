#include "Model.hpp"

#include <fmt/core.h>
#include <fmt/color.h>

#include "../App.hpp"

Model::Model() {

}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  const auto scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "Couldn't load the scene {}\n", path);
    return;
  }
  fmt::print("Proper file model load\n");
  if(scene->HasTextures()) {
    fmt::print("Model has  textures\n");
  }
  if(scene->HasMeshes()) {
    fmt::print("Model has meshes\n");
  }
  if(scene->HasMaterials()) {
    fmt::print("Model has materials\n");
  }

  for(uint32_t i = 0; i < scene->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[i];

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    vertices = parseVertices(mesh);
    indices = parseFaces(mesh);

    Mesh newMesh;
    newMesh.loadData(vertices, indices, {"colorPalette"});
    meshes.push_back(std::move(newMesh));
  }
}

std::vector<Vertex> Model::parseVertices(aiMesh*& mesh) {
  if(!mesh) {
    return {};
  }
  std::vector<Vertex> result;
  result.reserve(mesh->mNumVertices);

  for(uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.pos = {
      mesh->mVertices[i].x,
      mesh->mVertices[i].y,
      mesh->mVertices[i].z
    };
    vertex.normal = {
      mesh->mNormals[i].x,
      mesh->mNormals[i].y,
      mesh->mNormals[i].z,
    };
    if(mesh->mTextureCoords[0]) {
      vertex.uv = {
        mesh->mTextureCoords[0][i].x,
        mesh->mTextureCoords[0][i].y
      };
    } else {
      vertex.uv = {0.f, 0.f};
    }
    result.push_back(std::move(vertex));
  }
  return result;
}

std::vector<uint32_t> Model::parseFaces(aiMesh*& mesh) {
  if(!mesh) {
    return {};
  }
  std::vector<uint32_t> result;
  for(uint32_t i = 0; i < mesh->mNumFaces; i++) {
    auto face = mesh->mFaces[i];
    for(uint32_t j = 0; j < face.mNumIndices; j++) {
      result.emplace_back(face.mIndices[j]);
    }
  }
  
  return result;
}

void Model::render(Shader& shader,
                  const glm::mat4& model, 
                  const CameraPtr& camera) {
  for(auto& mesh : meshes) {
    mesh.render(shader, model, camera);
  }
}