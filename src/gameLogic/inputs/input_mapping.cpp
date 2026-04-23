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
}

bool ButtonMapping::pressed(GameInput input) const {
    return g_input.isDown(keys[input]);
}

bool ButtonMapping::justPressed(GameInput input) const {
    return g_input.isPressed(keys[input]);
}