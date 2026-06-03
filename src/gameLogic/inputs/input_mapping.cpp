#include "input_mapping.h"
#include "platform_input.h"

void ButtonMapping::init() {
    keys[MOVE_LEFT]  = GLFW_KEY_A;
    keys[MOVE_RIGHT] = GLFW_KEY_D;
    keys[MOVE_UP]    = GLFW_KEY_W;
    keys[MOVE_DOWN]  = GLFW_KEY_S;

    keys[WEAK_ATTACK]   = GLFW_KEY_U;
    keys[MEDIUM_ATTACK] = GLFW_KEY_I;
    keys[STRONG_ATTACK] = GLFW_KEY_O;

    keys[HITBOX_DEBUG] = GLFW_KEY_1;
    keys[CAMERA_MODE_CHANGE] = GLFW_KEY_2;

    keys[CAMERA_UP] = GLFW_KEY_0;
    keys[CAMERA_DOWN] = GLFW_KEY_9;
    keys[CAMERA_LEFT] = GLFW_KEY_LEFT;
    keys[CAMERA_RIGHT] = GLFW_KEY_RIGHT;
    keys[CAMERA_FORWARD] = GLFW_KEY_UP;
    keys[CAMERA_BACKWARD] = GLFW_KEY_DOWN;
}

bool ButtonMapping::pressed(GameInput input) const {
    return g_input.isDown(keys[input]);
}

bool ButtonMapping::justPressed(GameInput input) const {
    return g_input.isPressed(keys[input]);
}