#pragma once

#include <iostream>
#include <memory>

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
#include "graphics/camera/TopDownCamera.hpp"
#include "AssetManager.hpp"
#include "utility/GLUtils.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "Level.hpp"

class Level;

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
  double getTimeSinceStart() {
    return timeSinceStart();
  }
  const CameraPtr& getCamera() {
    return camera;
  }

  void setCameraPoint(glm::vec3 newPoint) {
    camerapoint = newPoint;
  }

  void close();
  void hideCursor();
  void showCursor();

  AssetManager assets;
  Input input;
private:
  void init();

  void initializeWindowContext();
  void initializeCallbacks();
  void initializeOpenGLOptions();
  void loadAssets();

  void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void windowSizeCallback(GLFWwindow* window, int width, int height);

  void loop();
  void inputHandler();
  void update();
  void render();
  void prepareRender();
  void finalizeRender();

  GLFWwindow* window;
  int width;
  int height;
  bool running;


  void showPerformanceInfo();
  Time::Timer timeSinceStart;
  const double dt = 1. / 60.;
  int tickCounter = 0;
  int fpsCounter = 0;

  std::unique_ptr<Level> level;
  CameraPtr camera;
  glm::vec3 camerapoint;

  friend class Input;
};

extern App* _App;
