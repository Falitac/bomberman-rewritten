#include "Mesh.hpp"

#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh() {

}

void Mesh::loadData(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
  create();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  
}

void Mesh::render(Shader& shader, const glm::mat4& model, CameraPtr& camera) {
  glBindVertexArray(vao);

  shader.use();
  auto view = camera->getView();
  auto projection = camera->getProjection();
  glUniformMatrix4fv(shader.findUniform("Model"), 1, false, glm::value_ptr(model));
  glUniformMatrix4fv(shader.findUniform("View"), 1, false, glm::value_ptr(view));
  glUniformMatrix4fv(shader.findUniform("Projection"), 1, false, glm::value_ptr(projection));
  glUniform3fv(shader.findUniform("CameraPos"), 1, glm::value_ptr(camera->getPosition()));

  glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr);
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
