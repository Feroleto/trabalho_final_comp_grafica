#include "platform_input.h"

PlatformInput g_input;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= MAX_KEYS) return;

    if (action == GLFW_PRESS)
        g_input.current[key] = true;
    else if (action == GLFW_RELEASE)
        g_input.current[key] = false;
}

void PlatformInput::initInput(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
}