#pragma once

#include <GL/glew.h>
#include <string>

enum class TextureType {
  Diffuse = 0,
  Specular,
  Normal,
};

class Texture {
public:
  Texture(const std::string& location = "");

  void loadFromFile(const std::string& location, TextureType textureType = TextureType::Diffuse);
  void setParameter(GLenum param, GLenum value);

  inline TextureType getType() { return textureType; }
  inline void use() {
    glBindTexture(GL_TEXTURE_2D, id);
  }
  inline GLuint operator()() { return id; }

  void destroy();
protected:
  GLuint id;

  TextureType textureType;

  void create();
};
