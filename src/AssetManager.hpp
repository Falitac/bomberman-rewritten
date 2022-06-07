#pragma once

#include <map>
#include <string>

#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Cubemap.hpp"

class AssetManager {
   public:
    AssetManager() = default;
    ~AssetManager();

    void addTexture(const std::string& name, const std::string& location, TextureType type = TextureType::Diffuse);
    void addCubemap(const std::string& name, const std::array<std::string, 6>& locations);
    void addShader(const std::string& name, const std::string& location);

    Texture& getTexture(const std::string& name);
    Cubemap& getCubemap(const std::string& name);
    Shader& getShader(const std::string& name);

   private:
    std::map<std::string, Texture> textures;
    std::map<std::string, Cubemap> cubemaps;
    std::map<std::string, Shader> shaders;
};
