#pragma once
#include "input_buffer.h"
#include "input_mapping.h"
#include "direction.h"
#include "input_types.h"

inline void readInput(ButtonMapping& mapping, InputBuffer& buffer) {
    int direction = resolveDirection(mapping);

    int buttons = Buttons::BUTTON_NONE;

    if (mapping.justPressed(WEAK_ATTACK))   buttons |= Buttons::LP;
    if (mapping.justPressed(MEDIUM_ATTACK)) buttons |= Buttons::MP;
    if (mapping.justPressed(STRONG_ATTACK)) buttons |= Buttons::HP;

    buffer.push(direction, buttons);
}