#include "App.hpp"

#include <glm/gtc/type_ptr.hpp>

static App* app;

static auto basicShaderLocation = std::string{"assets/shaders/basic"};

App::App()
: width(1600)
, height(900)
, running(true)
{
  app = this;
}

App::~App() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  basicShader.destroy();

  glfwTerminate();
}

void App::run() {
  init();
  loop();
}

void App::init() {
  initializeWindowContext();
  initializeCallbacks();
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  float vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
  };
  unsigned int indices[] = {
    0, 1, 2,
    0, 3, 1
  };

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

  try {
    basicShader = {basicShaderLocation};
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
  }
  
  camera = std::make_unique<FreeCamera>();

  timeSinceStart.restart();
}

void App::initializeWindowContext() {
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
}

void App::initializeCallbacks() {
  glfwSetWindowUserPointer(window, app);

  auto errorCallback = [](int error_code, const char* description) {
    fmt::print(fmt::fg(fmt::color::red), "GLFW error: {}\n", description);
  };
  auto mouseCallback = [](GLFWwindow* window, int button, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->mouseCallback(window, button, action, mods);
  };
  auto cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->windowSizeCallback(window, xpos, ypos);
  };
  auto keyboardCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->keyboardCallback(window, key, scancode, action, mods);
  };
  auto windowSizeCallback = [](GLFWwindow* window, int width, int height) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->windowSizeCallback(window, width, height);
  };

  glfwSetErrorCallback(errorCallback);
  glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetCursorPosCallback(window, cursorPosCallback);
  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void App::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
  app->input.setMouseButtonPressed(button, action);
}

void App::windowSizeCallback(GLFWwindow* window, double xpos, double ypos) {
  app->input.setMousePosition(xpos, ypos);
}

void App::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    app->input.setKeyPressed(key, true);
  }
  if(action == GLFW_RELEASE) {
    app->input.setKeyPressed(key, false);
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

  glViewport(0, 0, width, height);
  this->width = width;
  this->height = height;
}

void App::loop() {
  double timeAccumulator;
  double lastFrameTime = 0.0;
  Time::Timer timer, performanceInfoTimer;
  while(running) {
    timeAccumulator += timer.restart();
    inputHandler();

    while(timeAccumulator >= dt) {
      tickCounter++;
      update();
      timeAccumulator -= dt;
    }
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();

    if(performanceInfoTimer.count() > 1.) {
      performanceInfoTimer.restart();
      showPerformanceInfo();
    }
    fpsCounter++;
  }
  glfwDestroyWindow(window);
}

void App::inputHandler() {
  if(input.pressedKeys[GLFW_KEY_ESCAPE]) {
    close();
  }
  if(input.checkSinglePress(GLFW_KEY_Z)) {
    fmt::print(fmt::fg(fmt::color::beige), "Shader reload\n");
    basicShader.destroy();
    basicShader = {basicShaderLocation};
  }
  camera->handleInput(*this);
}

void App::close() {
  running = false;
}

void App::update() {

}

void App::showPerformanceInfo() {
  fmt::print("FPS: {}, Ticks: {}\n", fpsCounter, tickCounter); 
  fpsCounter = tickCounter = 0;
}

void App::render() {
  prepareRender();

  basicShader.use();
  auto view = camera->getView();
  auto projection = camera->getProjection(getAspect());

  auto uViewID = basicShader.findUniform("uView");
  auto uProjectionID = basicShader.findUniform("uProjection");

  glUniformMatrix4fv(uViewID, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uProjectionID, 1, GL_FALSE, glm::value_ptr(projection));

  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
}

void App::prepareRender() {
  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}