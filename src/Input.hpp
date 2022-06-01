#pragma once

#include <map>

#include <GLFW/glfw3.h>

class App;

class Input {
public:
    Input() = default;

    const std::map<int, bool>& getKeys() const;
    void setKeyPressed(int key, int value);
    void setMouseButtonPressed(int key, int value);
    bool checkSinglePress(int key);

    class Mouse {
    public:
    private:
        bool left;
        bool middle;
        bool right;
    };

private:
    friend class App;
    std::map<int, bool> pressedKeys;
    std::map<int, bool> keyStateChange;
    Mouse mouse;
};
