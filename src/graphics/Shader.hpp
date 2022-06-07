#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <exception>

#include <GL/glew.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <glm/glm.hpp>

class Shader {
private:
  GLuint programID;
  std::map<std::string, GLuint> variableLocations;
  std::map<std::string, GLuint> uniformLocations;
public:
  Shader() = default;
  Shader(const std::string& shaderName);

  const inline GLuint getProgramID() const {
    return programID;
  }
  const inline GLuint operator()() const {
    return programID;
  }
  inline void use() const {
    glUseProgram(programID);
  }

  void destroy();
  GLint findVar(const std::string& varName);
  GLint findUniform(const std::string& uniformName);

  void passMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
  std::string loadFromFileContent(const std::string& shaderName);
  void compile(const std::string& shaderName);
  GLuint compileShader(const std::string& shaderName, GLenum shaderType);

  class FileNotFound : public std::exception {
    std::string fileName;

    public:
    FileNotFound(const std::string& fileName)
    : fileName(fileName) {
    }

    virtual std::string what() final {
      return fmt::format("Can't load up: {}", fileName);
    }
  };
  class WrongShaderType : public std::exception {
    public:
    virtual std::string what() final {
      return "Wrong shader type!";
    }
  };
};


