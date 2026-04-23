#pragma once
#include "input_frame.h"

struct InputBuffer {
    static constexpr int SIZE = 30;

    InputFrame buffer[SIZE];
    int index = 0;

    void push(int dir, int btn) {
        buffer[index] = {dir, btn};
        index = (index + 1) % SIZE;
    }

    void push(InputFrame frame) {
        buffer[index] = frame;
        index = (index + 1) % SIZE;
    }

    void init() {
        InputFrame empty = {5, 0}; //neutral direction, no button
        for (int i = 0; i < SIZE; ++i) {
            buffer[i] = empty;
        }
    }

    InputFrame get(int i) const {
        int idx = (index - 1 - i + SIZE) % SIZE;
        return buffer[idx];
    }

    InputFrame getLast() const {
        return get(0);
    }
};