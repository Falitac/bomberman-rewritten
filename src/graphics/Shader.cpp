#include "Shader.hpp"

#include <string.h>
#include <iostream>
#include <array>
#include <memory>

#include <fmt/core.h>
#include <fmt/color.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& shaderName) {
  compile(shaderName);
}

void Shader::destroy() {
  glDeleteProgram(programID);
  variableLocations = {};
  uniformLocations = {};
}

std::string Shader::loadFromFileContent(const std::string& fileName) {
  std::ifstream file(fileName);
  std::stringstream s;

  if(file.good()) {
    s << file.rdbuf();
  } else {
    throw FileNotFound(fileName);
  }
  return s.str();
}

void Shader::compile(const std::string& shaderName) {
  programID = glCreateProgram();
  const static std::array<GLenum, 3> SHADER_TYPES = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER
  };

  for(auto& shaderType : SHADER_TYPES) {
    try {
      auto shader = compileShader(shaderName, shaderType);
      glAttachShader(programID, shader);
      glDeleteShader(shader);
    } catch(FileNotFound& e) {
      fmt::print("{}\n", e.what());
    }
  }

  glLinkProgram(programID);

  GLint compilationResult, errorLogSize;
  glGetProgramiv(programID, GL_LINK_STATUS, &compilationResult);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLogSize);

  if(errorLogSize > 0) {
    char message[1024];
    glGetProgramInfoLog(programID, errorLogSize, NULL, message);
    fmt::print(stderr, fmt::fg(fmt::color::red),
      "Error compiling program {}", message
    );
  }
}

GLuint Shader::compileShader(const std::string& shaderName, GLenum shaderType) {
  const static std::array<std::string, 3> EXTENSIONS = {".vert", ".frag", ".geom"};
  std::size_t fileExtensionID = 0;

  switch (shaderType) {
    case GL_VERTEX_SHADER:   fileExtensionID = 0; break;
    case GL_FRAGMENT_SHADER: fileExtensionID = 1; break;
    case GL_GEOMETRY_SHADER: fileExtensionID = 2; break;
    default: throw WrongShaderType(); break;
  }
  auto shaderSource = loadFromFileContent(shaderName + EXTENSIONS[fileExtensionID]);
  const char* shaderSourceCasted  = shaderSource.c_str();

  auto resultShader = glCreateShader(shaderType);
  glShaderSource(resultShader, 1, &shaderSourceCasted, NULL);
  glCompileShader(resultShader);

  GLint compilationResult, errorLogSize;
  glGetShaderiv(resultShader, GL_COMPILE_STATUS, &compilationResult);
  glGetShaderiv(resultShader, GL_INFO_LOG_LENGTH, &errorLogSize);

  if(errorLogSize > 0) {
    char message[1024];
    glGetShaderInfoLog(resultShader, errorLogSize, NULL, message);

    fmt::print(stderr, fmt::fg(fmt::color::red),
      "Compiling {} error:\n", shaderName + EXTENSIONS[fileExtensionID]
    );
    fmt::print(stderr, fmt::fg(fmt::color::blue_violet),
    "{}\n", message
    );
  }

  return resultShader;
}

GLint Shader::findVar(const std::string& varName) {
  if(!variableLocations.contains(varName)) {
    auto id = glGetAttribLocation(programID, varName.c_str());
    variableLocations[varName] = id;
  }
  return variableLocations[varName];
}

GLint Shader::findUniform(const std::string& uniformName) {
  if(!uniformLocations.contains(uniformName)) {
    auto id = glGetUniformLocation(programID, uniformName.c_str());
    variableLocations[uniformName] = id;
  }
  return variableLocations[uniformName];
}

void Shader::passMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
  glUniformMatrix4fv(findUniform("Model"), 1, false, glm::value_ptr(model));
  glUniformMatrix4fv(findUniform("View"), 1, false, glm::value_ptr(view));
  glUniformMatrix4fv(findUniform("Projection"), 1, false, glm::value_ptr(projection));
}

void Shader::passUniform(const std::string& name, const glm::mat2& data) {
  glUniformMatrix2fv(findUniform(name), 1, false, glm::value_ptr(data));
}

void Shader::passUniform(const std::string& name, const glm::mat3& data) {
  glUniformMatrix3fv(findUniform(name), 1, false, glm::value_ptr(data));
}

void Shader::passUniform(const std::string& name, const glm::mat4& data) {
  glUniformMatrix4fv(findUniform(name), 1, false, glm::value_ptr(data));
}

void Shader::passUniform(const std::string& name, const glm::vec2& data) {
  glUniform2fv(findUniform(name), 1, glm::value_ptr(data));
}

void Shader::passUniform(const std::string& name, const glm::vec3& data) {
  glUniform3fv(findUniform(name), 1, glm::value_ptr(data));
}

void Shader::passUniform(const std::string& name, const float& data) {
  glUniform1f(findUniform(name), data);
}