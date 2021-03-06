#include "Texture.hpp"

#include <fmt/core.h>
#include <fmt/color.h>
#include "../utility/stb_image.h"

#include "../utility/GLUtils.hpp"

Texture::Texture(const std::string& location) {
  if(location != "") {
    loadFromFile(location);
  }
}

void Texture::loadFromFile(const std::string& location, TextureType textureType) {
  this->textureType = textureType;
  int width, height, channels;
  auto data = stbi_load(location.c_str(), &width, &height, &channels, 4);
  if(!data) {
    fmt::print(fmt::fg(fmt::color::red), "Nie zaladuje tego {}\n", location);
    return;
  }
  create();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  checkGLerrors(__LINE__);

  stbi_image_free(data);
}

void Texture::setParameter(GLenum param, GLenum value) {
  use();
  glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::create() {
  glCreateTextures(GL_TEXTURE_2D, 1, &id);
  use();
}

void Texture::destroy() {
  glDeleteTextures(1, &id);
}

