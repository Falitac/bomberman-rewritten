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
  fmt::print("Test~App()\n");

  skybox.destroy();
  mesh.destroy();
  glfwTerminate();
}

void App::run() {
  init();
  loop();
  checkGLerrors(__LINE__);
}

void App::init() {
  initializeWindowContext();
  initializeOpenGLOptions();
  initializeCallbacks();
  //generateSimpleMesh();
  loadAssets();

  FreeCamera freeCamera({0.f, 1.f, 3.f});
  freeCamera.setSensitivity(0.08f);
  camera = std::make_unique<FreeCamera>(freeCamera);

  static std::vector<glm::vec3> pos = {
    {-1.f, -1.f, -1.f},
    {1.f, -1.f, -1.f},
    {1.f, 1.f, -1.f},
    {-1.f, 1.f, -1.f},
    {-1.f, -1.f, 1.f},
    {1.f, -1.f, 1.f},
    {1.f, 1.f, 1.f},
    {-1.f, 1.f, 1.f}
  };
  static std::vector<glm::vec3> normals = {
    {0.f, 0.f, 1.f},
    {1.f, 0.f, 0.f},
    {0.f, 0.f, -1.f},
    {-1.f, 0.f, 0.f},
    {0.f, 1.f, 0.f},
    {0.f, -1.f, 0.f},
  };
  std::vector<unsigned int> indices = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
  };
  std::vector<Vertex> vertices;

  for(int i = 0; i < 8; i++) {
    Vertex v{pos[i], normals[i], {0.f, 0.f}};
    vertices.push_back(std::move(v));
  }

  mesh.loadData(vertices, indices);

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

void App::loadAssets() {
  try {
    assets.addShader("basic", basicShaderLocation);
    assets.addShader("basic2", "assets/shaders/basic2");
    assets.addShader("skybox", "assets/shaders/skybox");
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
  }

  assets.addTexture("low-poly", "assets/textures/low-poly.png");

  /*
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
  assets.addCubemap("skybox-lightblue", {
    "assets/textures/lightblue/right.png",
    "assets/textures/lightblue/left.png",
    "assets/textures/lightblue/top.png",
    "assets/textures/lightblue/bot.png",
    "assets/textures/lightblue/front.png",
    "assets/textures/lightblue/back.png",
  });
  */
  assets.addCubemap("skybox-lightblue", {
    "assets/textures/lightblue/right.png",
    "assets/textures/lightblue/left.png",
    "assets/textures/lightblue/top.png",
    "assets/textures/lightblue/bot.png",
    "assets/textures/lightblue/front.png",
    "assets/textures/lightblue/back.png",
  });
  skybox.create();
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
      checkGLerrors(__LINE__);
    }
    fpsCounter++;
  }
}

void App::inputHandler() {
  if(input.pressedKeys[GLFW_KEY_ESCAPE]) {
    close();
  }
  if(input.checkSinglePress(GLFW_KEY_Z)) {
    fmt::print(fmt::fg(fmt::color::beige), "Shader reload\n");
    auto& basic = assets.getShader("basic2");
    basic.destroy();
    basic = {"assets/shaders/basic2"};
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

  mesh.render(assets.getShader("basic2"), glm::mat4{1.f}, camera);
  skybox.render(
    assets.getShader("skybox"),
    assets.getCubemap("skybox-lightblue"),
    camera
  );
}

void App::prepareRender() {
  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}