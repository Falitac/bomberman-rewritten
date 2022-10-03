#pragma once

#include <memory>
#include <vector>
#include <GL/glew.h>

#include "Vertex.hpp"
#include "camera/Camera.hpp"
#include "Shader.hpp"
#include "../AssetManager.hpp"

class Mesh {
public:
  Mesh();
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<GLuint>& indices, 
       const std::vector<std::string>& textures) {
        loadData(vertices, indices, textures);
  }

  void loadData(const std::vector<Vertex>& vertices,
                const std::vector<GLuint>& indices, 
                const std::vector<std::string>& textures);

  void render(Shader& shader);
  void destroy();
private:
  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  GLuint verticesCount;

  std::vector<std::string> appliedTextures;

  void passTexturesToShader(Shader& shader);
  void create();
};
