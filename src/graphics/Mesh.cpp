#include "Mesh.hpp"
#include "../App.hpp"

#include <glm/gtc/type_ptr.hpp>

App* _App;

Mesh::Mesh()
: vao(0)
, vbo(0)
, ebo(0)
{
}

void Mesh::loadData(const std::vector<Vertex>& vertices,
                    const std::vector<GLuint>& indices, 
                    const std::vector<std::string>& textures) {
  fmt::print("Kinda sus\n");
  create();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  verticesCount = indices.size();
  appliedTextures = textures;
}

void Mesh::render(Shader& shader,
                  const glm::mat4& model, 
                  const CameraPtr& camera) {
  glBindVertexArray(vao);

  shader.use();
  shader.passMVP(model, camera->getView(), camera->getProjection());
  glUniform3fv(shader.findUniform("CameraPos"), 1, glm::value_ptr(camera->getPosition()));

  passTexturesToShader(shader);

  glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

void Mesh::passTexturesToShader(Shader& shader) {
  auto& assets = _App->assets;
  unsigned diffuseId = 0;
  unsigned specularId = 0;
  unsigned normalId = 0;
  for(auto i = 0; i < appliedTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    auto& texture = assets.getTexture(appliedTextures[i]);

    std::string name = "Texture";
    switch(texture.getType()) {
    case TextureType::Diffuse:
      name += fmt::format("Diffuse{}", diffuseId++); break;
    case TextureType::Specular:
      name += fmt::format("Specular{}", specularId++); break;
    case TextureType::Normal:
      name += fmt::format("Normal{}", normalId++); break;
    default: name += "Default"; break;
    }

    glUniform1i(shader.findUniform(name), i);
    texture.use();
  }
}

void Mesh::destroy() {
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Mesh::create() {
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glCreateBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
}
