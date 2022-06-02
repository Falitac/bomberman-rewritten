#pragma once

#include <iostream>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utility/Timer.hpp"
#include "utility/stb_image.h"
#include "Input.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Skybox.hpp"
#include "graphics/camera/Camera.hpp"
#include "graphics/camera/FreeCamera.hpp"
#include "AssetManager.hpp"

class App {
public:
  App();
  ~App();

  void run();

public:
  inline int getWidth() {
      return width;
  }
  inline int getHeight() {
      return height;
  }
  inline float getAspect() {
      return static_cast<float>(width) / (height);
  }
  inline double getDt() {
      return dt;
  }
  void close();
  void hideCursor();
  void showCursor();

  friend class Input;
  Input input;
private:
  void init();

  void initializeWindowContext();
  void initializeOpenGLOptions();
  void initializeCallbacks();
  void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  void windowSizeCallback(GLFWwindow* window, double xpos, double ypos);
  void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void windowSizeCallback(GLFWwindow* window, int width, int height);

  void loop();
  void inputHandler();
  void update();
  void render();
  void prepareRender();

  GLFWwindow* window;
  int width;
  int height;
  bool running;


  void showPerformanceInfo();
  Time::Timer timeSinceStart;
  const double dt = 1. / 60.;
  int tickCounter = 0;
  int fpsCounter = 0;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  AssetManager assets;
  CameraPtr camera;
  std::unique_ptr<Skybox> skybox;
};
