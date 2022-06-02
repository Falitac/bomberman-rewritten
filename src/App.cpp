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
  fmt::print("Test~App()\n");

  glfwTerminate();
}

void App::run() {
  init();
  loop();
}

void App::init() {
  initializeWindowContext();
  initializeOpenGLOptions();
  initializeCallbacks();
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  float vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]), (void*) (3 * sizeof(float)));

  try {
    assets.addShader("basic", basicShaderLocation);
    assets.addShader("skybox", "assets/shaders/skybox");
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
  }
  
  FreeCamera freeCamera({0.f, 1.f, 3.f});
  freeCamera.setSensitivity(0.08f);
  camera = std::make_unique<FreeCamera>(freeCamera);

/*
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
*/

  assets.addTexture("low-poly", "assets/textures/low-poly.png");
  assets.addCubemap("skybox", {
    "assets/textures/penguins/arid_rt.jpg",
    "assets/textures/penguins/arid_lf.jpg",
    "assets/textures/penguins/arid_up.jpg",
    "assets/textures/penguins/arid_dn.jpg",
    "assets/textures/penguins/arid_bk.jpg",
    "assets/textures/penguins/arid_ft.jpg",
  });
  assets.addCubemap("skybox-blue", {
    "assets/textures/blue_skybox/bkg1_right.png",
    "assets/textures/blue_skybox/bkg1_left.png",
    "assets/textures/blue_skybox/bkg1_top.png",
    "assets/textures/blue_skybox/bkg1_bot.png",
    "assets/textures/blue_skybox/bkg1_front.png",
    "assets/textures/blue_skybox/bkg1_back.png",
  });
  skybox = std::make_unique<Skybox>();
  fmt::print("im Hreer\n");

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

void App::initializeOpenGLOptions() {
  glEnable(GL_DEPTH_TEST);
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
    auto& basic = assets.getShader("basic");
    basic.destroy();
    basic = {basicShaderLocation};
  }
}

void App::showPerformanceInfo() {
  fmt::print("FPS: {}, Ticks: {}\n", fpsCounter, tickCounter); 
  fpsCounter = tickCounter = 0;
}

void App::close() {
  running = false;
}

void App::hideCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void App::showCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
}

void App::update() {

  camera->handleInput(*this);
}

void App::render() {
  prepareRender();

  auto& basicShader = assets.getShader("basic");
  basicShader.use();
  auto view = camera->getView();
  auto projection = camera->getProjection();

  auto uViewID = basicShader.findUniform("uView");
  auto uProjectionID = basicShader.findUniform("uProjection");
  auto uTexture = basicShader.findUniform("Texture");

  assets.getTexture("low-poly").use();

  glUniformMatrix4fv(uViewID, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(uProjectionID, 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  auto& skyboxCubemap = assets.getCubemap("skybox-blue");
  auto& skyboxShader = assets.getShader("skybox");
  skybox->render(skyboxShader, skyboxCubemap, camera);
}

void App::prepareRender() {
  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}