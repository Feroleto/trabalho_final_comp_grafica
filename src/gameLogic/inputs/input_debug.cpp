#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <string>
#include "input_debug.h"

std::string InputDebugRenderer::buildString(const InputSystem& input)
{
    const InputBuffer& buffer = input.getBuffer();

    std::string text = "Inputs: ";

    for (int i = 0; i < visibleFrames; i++)
    {
        InputFrame f = buffer.get(i);

        //if (f.direction == 5 && f.buttons == 0) continue;

        text += "[";

        char tmp[16];
        sprintf_s(tmp, "%d", (int)f.direction);
        text += tmp;

        if (f.buttons & Buttons::LP) text += " LP";
        if (f.buttons & Buttons::MP) text += " MP";
        if (f.buttons & Buttons::HP) text += " HP";

        text += "] ";
    }

    return text;
}