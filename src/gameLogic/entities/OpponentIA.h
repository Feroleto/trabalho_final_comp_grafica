#ifndef OPPONENT_IA_H
#define OPPONENT_IA_H
#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <glm/vec3.hpp>
#include "../collision_system/utilStructures.h"
#include "..\..\character\animatedModel.h"

void updateOpponentIA(
    float deltaTime,
    float currentTime,
    float& g_OpponentForcedAnimationEnd,
    float& g_OpponentX, float& g_OpponentY, float& g_OpponentZ,
    float& g_OpponentAnimationTime,
    float& g_OpponentAnimationStartTime,
    float& g_OpponentStartX, float& g_OpponentStartZ,
    std::string& g_OpponentCurrentAnimation,
    bool& g_Proj1OpponentSpawned,
    bool& g_Proj2OpponentSpawned,
    bool& g_Proj3OpponentSpawned,
    const float g_CharacterX, const float g_CharacterY, const float g_CharacterZ,
    AnimatedModel& g_Opponent)
{
    glm::vec3 toPlayer(g_CharacterX - g_OpponentX, 0.0f, g_CharacterZ - g_OpponentZ);

    float distToPlayer = glm::length(toPlayer);

    glm::vec3 forward =
        (distToPlayer > 0.001f)
        ? glm::normalize(toPlayer)
        : glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 right =
        glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distri(0, 99);

    int r = distri(rng);

    enum class OpponentState {
        IDLE,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    static OpponentState currentState = OpponentState::IDLE;
    static float nextDecisionTime = 0.0f;
    static float nextAttackTime = 0.0f;

    if (currentTime >= nextAttackTime && currentTime >= g_OpponentForcedAnimationEnd) {
        if (distToPlayer <= 2.0f) {
            float dur = 0.7f;

            g_OpponentStartX = g_OpponentX;
            g_OpponentStartZ = g_OpponentZ;

            g_OpponentCurrentAnimation = "sword_combo";
            g_OpponentAnimationTime = 0.9f;
            g_OpponentForcedAnimationEnd = currentTime + dur;

            nextAttackTime = currentTime + 2.5f; //cooldown 2.5 segundos
            return;
        }
        if (distToPlayer > 5.0f) {
            float dur = g_Opponent.getAnimationDuration("triple_slash_attack");

            if (dur <= 0.0f)
                dur = 1.0f;

            g_OpponentStartX = g_OpponentX;
            g_OpponentStartZ = g_OpponentZ;

            g_OpponentAnimationStartTime = currentTime;
            g_OpponentCurrentAnimation = "triple_slash_attack";
            g_OpponentAnimationTime = 0.0f;
            g_OpponentForcedAnimationEnd = currentTime + dur;

            g_Proj1OpponentSpawned = false;
            g_Proj2OpponentSpawned = false;
            g_Proj3OpponentSpawned = false;
            
            nextAttackTime = currentTime + 7.0f;
            return;
        }
    }

    if (currentTime >= nextDecisionTime) {
        std::uniform_real_distribution<float> timerDist(0.5f, 1.0f);
        nextDecisionTime = currentTime + timerDist(rng);

        std::uniform_int_distribution<int> r(0, 99);
        int roll = r(rng);

        if (distToPlayer > 2.0f) {
            //longe 70% chance de aproximar, 20% girar, 10% recuar
            if (roll < 70)       currentState = OpponentState::FORWARD;
            else if (roll < 90)  currentState = (roll % 2 == 0) ? OpponentState::LEFT
                                                                : OpponentState::RIGHT;
            else                 currentState = OpponentState::BACKWARD;
        } else {
            //perto 40 girar, 30% recuar
            if (roll < 50)       currentState = (roll % 2 == 0) ? OpponentState::LEFT
                                                                : OpponentState::RIGHT;
            else if (roll < 70)  currentState = OpponentState::BACKWARD;
            else                 currentState = OpponentState::IDLE;
        }
    }

    switch (currentState) {
        case OpponentState::FORWARD:
            g_OpponentX += forward.x * 2.0f * deltaTime;
            g_OpponentZ += forward.z * 2.0f * deltaTime;
            g_OpponentCurrentAnimation = "walk_forward";
            break;
        case OpponentState::BACKWARD:
            g_OpponentX -= forward.x * 2.0f * deltaTime;
            g_OpponentZ -= forward.z * 2.0f * deltaTime;
            g_OpponentCurrentAnimation = "walk_backwards";
            break;
        case OpponentState::LEFT:
            g_OpponentX += right.x * 2.0f * deltaTime;
            g_OpponentZ += right.z * 2.0f * deltaTime;
            g_OpponentCurrentAnimation = "strafe_left";
            break;
        case OpponentState::RIGHT:
            g_OpponentX -= right.x * 2.0f * deltaTime;
            g_OpponentZ -= right.z * 2.0f * deltaTime;
            g_OpponentCurrentAnimation = "strafe_right";
            break;
        default:
            g_OpponentCurrentAnimation = "idle";
            break;
    }
}


#endif