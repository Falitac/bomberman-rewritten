#pragma once

#include <map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class App;

enum class MButton {
    Left,
    Right,
    Middle
};

class Input {
public:
    Input() = default;

    void setKeyPressed(int key, int value);
    void setMouseButtonPressed(int key, int value);
    void setMousePosition(double xpos, double ypos);
    void centerizeMouse(App& app);

    bool isKeyPressed(int key);
    bool isMouseButtonPressed(MButton button);
    bool checkSinglePress(int key);

    const std::map<int, bool>& getKeys() const;
    glm::vec2 getMousePosition() const {
        return {mouse.xpos, mouse.ypos};
    }
    glm::vec2 getMouseOffset() {
        auto xoffset = mouse.xposLast - mouse.xpos;
        auto yoffset = mouse.yposLast - mouse.ypos;
        mouse.xposLast = mouse.xpos;
        mouse.yposLast = mouse.ypos;
        return {xoffset, yoffset};
    }
private:
    friend class App;
    std::map<int, bool> pressedKeys;
    std::map<int, bool> keyStateChange;

    struct Mouse {
        bool left;
        bool middle;
        bool right;

        double xpos;
        double ypos;
        double xposLast;
        double yposLast;
    } mouse;
};
