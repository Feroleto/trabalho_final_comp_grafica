#include "input_system.h"
#include "input_reader.h"
#include "platform_input.h"

void InputSystem::init(GLFWwindow* window) {
    g_input.initInput(window);
    buffer.init();
    mapping.init();
}

void InputSystem::update() {
    readInput(mapping, buffer);
    g_input.update();
}

/*void InputSystem::updateWithHistory() {
    readInput(mapping, buffer);

    InputFrame lastBuffer = buffer.getLast();
    InputFrame lastHistory = history.getLast();

    if (lastHistory.direction != lastBuffer.direction ||
        lastHistory.buttons != lastBuffer.buttons) {

        history.push(lastBuffer.direction, lastBuffer.buttons);
    }
}*/

const InputBuffer& InputSystem::getBuffer() const {
    return buffer;
}

/*const InputBuffer& InputSystem::getHistory() const {
    return history;
}*/