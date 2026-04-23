#pragma once
#include <GLFW/glfw3.h>
#include <cstring>

constexpr int MAX_KEYS = 512;

struct PlatformInput {
    bool current[MAX_KEYS] = {};
    bool previous[MAX_KEYS] = {};

    void initInput(GLFWwindow* window);

    void update() {
        std::memcpy(previous, current, sizeof(current));
    }

    bool isDown(int key) const {
        return current[key];
    }

    bool isPressed(int key) const {
        return current[key] && !previous[key];
    }
};

extern PlatformInput g_input;