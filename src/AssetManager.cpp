#include "AssetManager.hpp"

AssetManager::~AssetManager() {
  fmt::print("Test~Asset()\n");
  for(auto& [name, texture] : textures) {
    texture.destroy();
  }
  for(auto& [name, shader] : shaders) {
    shader.destroy();
  }
  for(auto& [name, cubemap] : cubemaps) {
    cubemap.destroy();
  }
}

void AssetManager::addTexture(const std::string& name,
                              const std::string& location) {
  if(textures.contains(name)) {
    fmt::print(fmt::fg(fmt::color::sea_green), "Texture {} already exists\n");
    return;
  }
  Texture texture(location);
  textures.insert(std::make_pair(name, texture));
}

Texture& AssetManager::getTexture(const std::string& name) {
  return textures[name];
}

void AssetManager::addCubemap(const std::string& name,
                              const std::array<std::string, 6>& locations) {
  if(cubemaps.contains(name)) {
    fmt::print(fmt::fg(fmt::color::sea_green), "Cubemap {} already exists\n");
    return;
  }
  Cubemap cubemap;
  cubemap.loadFromFile(locations);
  cubemaps.insert(std::make_pair(name, cubemap));
}

Cubemap& AssetManager::getCubemap(const std::string& name) {
  return cubemaps[name];
}

void AssetManager::addShader(const std::string& name,
                             const std::string& location) {
  if(shaders.contains(name)) {
    fmt::print(fmt::fg(fmt::color::sea_green), "Shader {} already exists\n");
    return;
  }
  Shader shader = {location};
  shaders.insert(std::make_pair(name, shader));
}

Shader& AssetManager::getShader(const std::string& name) {
  return shaders[name];
}
