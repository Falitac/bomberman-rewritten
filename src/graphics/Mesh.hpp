#pragma once

#include <vector>
#include <GL/glew.h>
#include "Vertex.hpp"
#include "camera/Camera.hpp"
#include "Shader.hpp"

class Mesh {
public:
  Mesh();

  void loadData(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

  void render(Shader& shader, const glm::mat4& model, CameraPtr& camera);
  void destroy();
private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  GLuint verticesCount;

  void create();
};
