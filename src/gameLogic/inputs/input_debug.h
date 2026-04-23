#pragma once
#include "input_system.h"
#include <GLFW/glfw3.h>

class InputDebugRenderer {
public:
    GLuint texture;
    float x = 20.0f;
    float y = 20.0f;
    float size = 32.0f;
    int visibleFrames = 10;

    std::string buildString(const InputSystem& input);

};
