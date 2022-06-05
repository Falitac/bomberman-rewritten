#include "GLUtils.hpp"

#include <GL/glew.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <string>

static std::string getErrorName(GLenum glError);

void checkGLerrors(int line) {
  GLenum glError;
  while((glError = glGetError()) != GL_NO_ERROR) {
    fmt::print("Line error: {}\n", line);
    auto color = fmt::fg(fmt::color::red);
    fmt::print(color, "{}\n", getErrorName(glError));
  }
}

static std::string getErrorName(GLenum glError) {
  switch (glError) {
  case GL_INVALID_ENUM: return {"GL_INVALID_ENUM"};
    break;
  case GL_INVALID_VALUE: return {"GL_INVALID_VALUE"};
    break;
  case GL_INVALID_OPERATION: return {"GL_INVALID_OPERATION"};
    break;
  case GL_STACK_OVERFLOW: return {"GL_STACK_OVERFLOW"};
    break;
  case GL_STACK_UNDERFLOW: return {"GL_STACK_UNDERFLOW"};
    break;
  case GL_OUT_OF_MEMORY: return {"GL_OUT_OF_MEMORY"};
    break;
  default:
    return {"Unknown error"};
    break;
  }
}