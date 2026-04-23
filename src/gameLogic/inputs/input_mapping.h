#pragma once
#include "input_types.h"

struct ButtonMapping {
    // Se o erro INPUT_COUNT persistir, tente usar GameInput::INPUT_COUNT
    int keys[GameInput::INPUT_COUNT]; 

    void init();

    bool pressed(GameInput input) const;
    bool justPressed(GameInput input) const;
};