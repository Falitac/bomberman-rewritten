#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
public:
  Texture(const std::string& location = "");

  void loadFromFile(const std::string& location);
  inline void use() {
    glBindTexture(GL_TEXTURE_2D, id);
  }
  inline GLuint operator()() { return id; }

  void destroy();
protected:
  GLuint id;

  void create();
};
