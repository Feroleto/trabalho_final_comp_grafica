#pragma once

#include <GLFW/glfw3.h>
#include "input_mapping.h"
#include "input_buffer.h"

// Forward declaration (evita include desnecessário no header)
struct InputFrame;

struct InputSystem {
    ButtonMapping mapping;
    InputBuffer buffer;
    /*InputBuffer history;*/

    void init(GLFWwindow* window);

    void update();
    //void updateWithHistory();

    const InputBuffer& getBuffer() const;
    //const InputBuffer& getHistory() const;
};