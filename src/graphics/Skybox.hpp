#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Cubemap.hpp"
#include "Shader.hpp"

#include "camera/Camera.hpp"

class Skybox {
public:
  Skybox();

  void render(Shader& shader, Cubemap& cubemap,
              const CameraPtr& camera);
  void create();
  void destroy();
private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};
