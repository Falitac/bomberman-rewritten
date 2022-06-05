#include "Skybox.hpp"

#include <array>

#include <glm/gtc/type_ptr.hpp>

Skybox::Skybox() {
}

void Skybox::render(Shader& shader, Cubemap& cubemap,
                    const CameraPtr& camera) {
  glDepthFunc(GL_LEQUAL);
  glBindVertexArray(vao);
  
  cubemap.use();
  glActiveTexture(GL_TEXTURE0);
  shader.use();

  auto view = glm::mat4{glm::mat3{camera->getView()}};
  auto projection = camera->getProjection();
  auto uViewID = shader.findUniform("View");
  auto uProjectionID = shader.findUniform("Projection");

  glUniformMatrix4fv(uViewID, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uProjectionID, 1, GL_FALSE, glm::value_ptr(projection));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
  glDepthFunc(GL_LESS);
}

void Skybox::create() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  constexpr std::array<glm::vec3, 8> vertices = {{
    {-1.f, -1.f, -1.f},
    {1.f, -1.f, -1.f},
    {1.f, 1.f, -1.f},
    {-1.f, 1.f, -1.f},
    {-1.f, -1.f, 1.f},
    {1.f, -1.f, 1.f},
    {1.f, 1.f, 1.f},
    {-1.f, 1.f, 1.f}
  }};
  constexpr std::array<GLuint, 36> indices = {{
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
  }};

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertices[0]), nullptr);

  glBindVertexArray(0);
}

void Skybox::destroy() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}
