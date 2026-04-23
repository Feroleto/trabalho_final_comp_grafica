#pragma once
#include "input_mapping.h"

inline int resolveDirection(const ButtonMapping& input) {
    /*bool left  = input.pressed(MOVE_LEFT);
    bool right = input.pressed(MOVE_RIGHT);
    bool up    = input.pressed(MOVE_UP);
    bool down  = input.pressed(MOVE_DOWN);*/

    if (input.pressed(MOVE_DOWN) && input.pressed(MOVE_LEFT)) return 1;
    if (input.pressed(MOVE_DOWN) && input.pressed(MOVE_RIGHT)) return 3;
    if (input.pressed(MOVE_UP) && input.pressed(MOVE_LEFT)) return 7;
    if (input.pressed(MOVE_UP) && input.pressed(MOVE_RIGHT)) return 9;

    if (input.pressed(MOVE_DOWN)) return 2;
    if (input.pressed(MOVE_UP)) return 8;
    if (input.pressed(MOVE_LEFT)) return 4;
    if (input.pressed(MOVE_RIGHT)) return 6;

    return 5;
}