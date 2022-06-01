#pragma once

#include <iostream>

#include <fmt/core.h>
#include <fmt/color.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

private:
    void init();
    void initializeCallbacks();
    void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void windowSizeCallback(GLFWwindow* window, int width, int height);

    void loop();
    void update();
    void render();

    GLFWwindow* window;
    int width;
    int height;
    bool running;
};
