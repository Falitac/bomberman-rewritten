#include "App.hpp"

#include "Level.hpp"
#include <glm/gtc/type_ptr.hpp>


App::App()
: width(1600)
, height(900)
, running(true)
{
  _App = this;
}

App::~App() {
  fmt::print("Test~App()\n");
  glfwTerminate();
}

void App::run() {
  init();
  loop();
  checkGLerrors(__LINE__);
}

void App::init() {
  fmt::print("Before window init\n");
  initializeWindowContext();
  initializeOpenGLOptions();
  initializeCallbacks();
  loadAssets();

  FreeCamera freeCamera({0.f, 1.f, 3.f});
  freeCamera.setSensitivity(0.08f);
  camera = std::make_unique<FreeCamera>(freeCamera);
  TopDownCamera topdownCamera(camerapoint);

  level = std::make_unique<Level>(25, 25, 2.f);

  timeSinceStart.restart();
  fmt::print("Init finished, starting program\n");
}

void App::initializeWindowContext() {
  int glfwStatus = glfwInit();
  if(!glfwStatus) {
    throw std::runtime_error("GLFW error");
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);
  window = glfwCreateWindow(width, height, "Bomberman", nullptr, nullptr);
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
  auto baseTexturePath = std::string{"assets/textures/"};

  try {
    auto basicShaderLocation = std::string{"assets/shaders/basic"};
    assets.addShader("basic", basicShaderLocation);
    assets.addShader("basic2", "assets/shaders/basic2");
    assets.addShader("skybox", "assets/shaders/skybox");
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
  }

  assets.addTexture("colorPalette", "assets/textures/color-palette.png");
  assets.addTexture("wood-diff", "assets/textures/wood/wood_table_001_diff_4k.jpg");
  assets.addTexture("wood-spec", "assets/textures/wood/wood_table_001_rough_4k.jpg", TextureType::Specular);
  assets.addTexture("wood-norm", "assets/textures/wood/wood_table_001_nor_gl_4k.jpg", TextureType::Normal);

  assets.addCubemap("skybox-lightblue", {
    "assets/textures/lightblue/right.png",
    "assets/textures/lightblue/left.png",
    "assets/textures/lightblue/top.png",
    "assets/textures/lightblue/bot.png",
    "assets/textures/lightblue/front.png",
    "assets/textures/lightblue/back.png",
  });
  checkGLerrors(__LINE__);
}

void App::loop() {
  double timeAccumulator;
  double lastFrameTime = 0.0;
  Time::Timer timer, performanceInfoTimer;
  while(running) {
    glfwPollEvents();
    timeAccumulator += timer.restart();
    inputHandler();

    bool shouldRender = false;
    while(timeAccumulator >= dt) {
      tickCounter++;
      update();
      timeAccumulator -= dt;
      shouldRender = true;
    }
    if(shouldRender) {
      render();
      fpsCounter++;
    }

    if(performanceInfoTimer.count() > 1.) {
      performanceInfoTimer.restart();
      showPerformanceInfo();
      checkGLerrors(__LINE__);
    }
  }
}

void App::inputHandler() {
  if(input.pressedKeys[GLFW_KEY_ESCAPE]) {
    close();
  }
  if(input.checkSinglePress(GLFW_KEY_Z)) {
    fmt::print(fmt::fg(fmt::color::beige), "Shader reload\n");
    auto& basic = assets.getShader("basic");
    basic.destroy();
    basic = {"assets/shaders/basic"};

    auto& basic2 = assets.getShader("basic2");
    basic2.destroy();
    basic2 = {"assets/shaders/basic2"};

    auto& skyboxShader = assets.getShader("skybox");
    skyboxShader.destroy();
    skyboxShader = {"assets/shaders/skybox"};

    auto skyboxUniform = basic2.findUniform("skybox");
    auto diffuseUniform = basic2.findUniform("TextureDiffuse0");
    fmt::print("SkyboxUniform.{}\n", skyboxUniform);
    fmt::print("DiffuseUniform.{}\n", diffuseUniform);
  }
  if(input.checkSinglePress(GLFW_KEY_O)) {
    FreeCamera freeCamera({0.f, 1.f, 3.f}, 50.f);
    freeCamera.setSensitivity(0.08f);
    camera = std::make_unique<FreeCamera>(freeCamera);
  }
  if(input.checkSinglePress(GLFW_KEY_P)) {
    TopDownCamera topdownCamera(camerapoint, 80.f, 7.f);
    camera = std::make_unique<TopDownCamera>(topdownCamera);
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
  level->update();
  camera->setAspect(getAspect());
  camera->update();
}

void App::render() {
  prepareRender();
  level->render();
  finalizeRender();
}

void App::prepareRender() {
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::finalizeRender() {
  glfwSwapBuffers(window);
}