#include "App.hpp"

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
  loadAssets();

  FreeCamera freeCamera({0.f, 1.f, 3.f});
  freeCamera.setSensitivity(0.08f);
  camera = std::make_unique<FreeCamera>(freeCamera);

  std::vector<glm::vec3> pos = {
    {-1.f, -1.f, -1.f},
    {1.f, -1.f, -1.f},
    {1.f, 1.f, -1.f},
    {-1.f, 1.f, -1.f},
    {-1.f, -1.f, 1.f},
    {1.f, -1.f, 1.f},
    {1.f, 1.f, 1.f},
    {-1.f, 1.f, 1.f}
  };
  std::vector<glm::vec3> normals(8);
  for(int i = 0; i < pos.size(); i++) {
    normals[i] = glm::normalize(pos[i]);
  }
  std::vector<unsigned int> indices = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
  };
  std::vector<glm::vec2> uvs = {
    {0.0, 0.0},
    {0.0, 1.0},
    {1.0, 1.0},
    {1.0, 0.0},
    {0.0, 0.0},
    {0.0, 1.0},
    {1.0, 1.0},
    {1.0, 0.0},
  };
  std::vector<Vertex> vertices;

  for(int i = 0; i < pos.size(); i++) {
    Vertex v{pos[i], normals[i], uvs[i]};
    vertices.push_back(std::move(v));
  }

  mesh.loadData(vertices, indices, {"wood-diff", "wood-spec", "wood-norm"});

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
    auto basicShaderLocation = std::string{"assets/shaders/basic"};
    assets.addShader("basic", basicShaderLocation);
    assets.addShader("basic2", "assets/shaders/basic2");
    assets.addShader("skybox", "assets/shaders/skybox");
  } catch(const std::exception& e) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}", e.what());
  }

  assets.addTexture("low-poly", "assets/textures/low-poly.png");

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

  model.loadModel("assets/objects/cube.obj");

  skybox.create();
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
    auto& basic = assets.getShader("basic2");
    basic.destroy();
    basic = {"assets/shaders/basic2"};

    auto& skyboxShader = assets.getShader("skybox");
    skyboxShader.destroy();
    skyboxShader = {"assets/shaders/skybox"};
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

  //mesh.render(assets.getShader("basic2"), glm::mat4{1.f}, camera);
  model.render(assets.getShader("basic2"), glm::mat4{1.f}, camera);
  skybox.render(
    assets.getShader("skybox"),
    assets.getCubemap("skybox-lightblue"),
    camera
  );
  finalizeRender();
}

void App::prepareRender() {
  glClearColor(0.1, 0.2, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::finalizeRender() {
  glfwSwapBuffers(window);
}