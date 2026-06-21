#pragma once

enum GameInput {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,

    WEAK_ATTACK,
    MEDIUM_ATTACK,
    STRONG_ATTACK,

    HITBOX_DEBUG,
    CAMERA_MODE_CHANGE,
    OPPONENT_AI,

    CAMERA_UP,
    CAMERA_DOWN,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_FORWARD,
    CAMERA_BACKWARD,

    INPUT_COUNT
};

enum Buttons {
    BUTTON_NONE = 0,
    LP = 1 << 0,
    MP = 1 << 1,
    HP = 1 << 2
};