#include "Cubemap.hpp"

#include <fmt/core.h>
#include <fmt/color.h>
#include "../utility/stb_image.h"

Cubemap::Cubemap() {
}

void Cubemap::loadFromFile(const std::array<std::string, 6>& locations) {
  int width, height, channels;

  create();
  for(auto i = 0; i < 6; i++) {
    auto location = locations[i];
    auto data = stbi_load(location.c_str(), &width, &height, &channels, 4);
    if(!data) {
      fmt::print(fmt::fg(fmt::color::red), "Cubemap {}, doesn't exist\n", location);
      destroy();
      return;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}

void Cubemap::create() {
  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
  use();
}

void Cubemap::destroy() {
  glDeleteTextures(1, &id);
}


