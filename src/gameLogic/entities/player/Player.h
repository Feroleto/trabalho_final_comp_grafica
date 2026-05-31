#pragma once
#include "../Character.h"
#include "../../inputs/input_system.h"
#include "PlayerInput.h"
#include <cstdio>

class Player : public Character {
    InputSystem inputSystem;
    PlayerInput inputHandler;

    public:

    PlayerAction actionIntention = PlayerAction::NONE;

    Player(GLFWwindow* window) {
        printf("DEBUG: Initializing InputSystem...\n");
        fflush(stdout);
        inputSystem.init(window);
        printf("DEBUG: InputSystem initialized successfully.\n");
        fflush(stdout);
    }

    void update(float delta, Object* enemyObject) {
        inputSystem.update();
        actionIntention = inputHandler.update(inputSystem.getBuffer());

        if (actionIntention == PlayerAction::WALK_DOWN_LEFT) {
            transform.position.x -= 1.0f * delta;
            transform.position.z -= 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_DOWN) {
            transform.position.z -= 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_DOWN_RIGHT) {
            transform.position.x += 1.0f * delta;
            transform.position.z -= 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_RIGHT) {
            transform.position.x += 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_UP_RIGHT) {
            transform.position.x += 1.0f * delta;
            transform.position.z += 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_UP) {
            transform.position.z += 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_UP_LEFT) {
            transform.position.x -= 1.0f * delta;
            transform.position.z += 1.0f * delta;
            transform.dirty = true;
        } else if (actionIntention == PlayerAction::WALK_LEFT) {
            transform.position.x -= 1.0f * delta;
            transform.dirty = true;
        }

        if(actionIntention == PlayerAction::FIREBALL) {
            if(!projectiles[0].isActive && !projectiles[1].isActive) {
                spawnBezierProjectiles(this, enemyObject, &projectiles[0], &projectiles[1]);
            }
        }

        updateBezier(nullptr, &projectiles[0], &projectiles[1]);

        Character::update();
    }

};