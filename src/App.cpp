#include "App.hpp"

App::App()
: width(1600)
, height(900)
, running(true)
{

}

App::~App() {
  glfwTerminate();
}

void App::run() {
  init();
  loop();
}

void App::init() {
  int glfwStatus = glfwInit();
  if(!glfwStatus) {
    throw std::runtime_error("GLFW error");
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);
  window = glfwCreateWindow(width, height, "App", nullptr, nullptr);
  if(!window) {
    throw std::runtime_error("Window cannot be initialized");
  }
  glfwMakeContextCurrent(window);

  glewExperimental = true;
  GLenum glewStatus = glewInit();
  if(glewStatus != GLEW_OK) {
    throw std::runtime_error("GLEW error");
  }
  glfwSwapInterval(1);

  initializeCallbacks();
}

void App::initializeCallbacks() {
  App* app;
  glfwSetWindowUserPointer(window, app);

  auto errorCallback = [](int error_code, const char* description) {
    fmt::print(fmt::fg(fmt::color::red), "GLFW error\n");
  };
  auto mouseCallback = [](GLFWwindow* window, int button, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->mouseCallback(window, button, action, mods);
  };
  auto keyboardCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->keyboardCallback(window, key, scancode, action, mods);
  };
  auto windowSizeCallback = [](GLFWwindow* window, int width, int height) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->windowSizeCallback(window, width, height);
  };

  glfwSetErrorCallback(errorCallback);
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void App::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
  fmt::print(fmt::fg(fmt::color::yellow), "Mouse button\n");
}

void App::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  fmt::print(fmt::fg(fmt::color::yellow), "Keyboard\n");
  if(action == GLFW_PRESS) {
    if(key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  }
}

void App::windowSizeCallback(GLFWwindow* window, int width, int height) {
  static int printCounter;
  if(printCounter > 20) {
    printCounter = 0;
    fmt::print(fmt::fg(fmt::color::aquamarine),
      "Window new dimensions {}x{}\n", width, height);
  }
  printCounter++;
}

void App::loop() {
  while(!glfwWindowShouldClose(window)) {
    update();
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void App::update() {

}

void App::render() {

}