#pragma once

#include "Object.h"

class Character : public Object {
public:
    float life = 1000.0f;
    bool isPlayer = false;

};