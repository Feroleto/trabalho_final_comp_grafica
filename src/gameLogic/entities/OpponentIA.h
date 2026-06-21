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

    if (distToPlayer <= 3.0f && r <= 50)
    {
        float dur = 0.7f;

        g_OpponentStartX = g_OpponentX;
        g_OpponentStartZ = g_OpponentZ;

        g_OpponentCurrentAnimation = "sword_combo";
        g_OpponentAnimationTime = 0.9f;
        g_OpponentForcedAnimationEnd = currentTime + dur;

        return;
    }

    if (distToPlayer > 10.0f && r <= 70)
    {
        float dur =
            g_Opponent.getAnimationDuration(
                "triple_slash_attack"
            );

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

        return;
    }

    // ======================
    // Movimento
    // ======================
    glm::vec3 movement(0.0f);

    if (distToPlayer >= 1.0f)
    {
        if (g_OpponentCurrentAnimation == "walk_forward")
            r -= 10;

        if (g_OpponentCurrentAnimation == "walk_backwards")
            r += 10;

        if (r <= 40)
        {
            movement += forward;
            g_OpponentCurrentAnimation = "walk_forward";
        }
        else if (r >= 60)
        {
            movement -= forward;
            g_OpponentCurrentAnimation = "walk_backwards";
        }
        else if (r <= 50)
        {
            movement += right;
            g_OpponentCurrentAnimation = "strafe_left";
        }
        else
        {
            movement -= right;
            g_OpponentCurrentAnimation = "strafe_right";
        }

        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);

            g_OpponentX += movement.x * 0.5f * deltaTime;
            g_OpponentZ += movement.z * 0.05f * deltaTime;
        }
    }
}

#endif