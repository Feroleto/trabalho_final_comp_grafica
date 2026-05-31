#pragma once

#include "Object.h"
#include "../attacks/attacks.h"

class Character : public Object {
public:

    Character() {
        projectiles.resize(2); 
        
        for(auto& proj : projectiles) {
            proj.isActive = false;
            proj.currentFrame = 0;
            proj.hitbox.localTransform.position = this->transform.position;
            proj.hitbox.localTransform.dirty = true;
            proj.hitbox.update(glm::mat4({1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f}));
        }
    }

    float life = 1000.0f;
    bool isPlayer = false;
    std::vector<Projectile> projectiles;

};