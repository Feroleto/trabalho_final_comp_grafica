#pragma once

#include "../entities/Character.h"

    // PRE-CONDITION: bodies must be sorted by globalAABB.min.x (ascending)
inline void queryObjectObject(Object* reference,
                    const std::vector<Object*>& bodies,
                    std::vector<Object*>& candidates) {
    candidates.clear();

    for (size_t i = 0; i < bodies.size(); i++) {
        Object* b = bodies[i];

        if (b == reference)
            continue;

        // b está completamente à esquerda
        if (b->globalAABB.max.x < reference->globalAABB.min.x)
            continue;

        // b está completamente à direita, então pode parar
        if (b->globalAABB.min.x > reference->globalAABB.max.x)
            break;

        // checagem no eixo Y
        if (b->globalAABB.min.y > reference->globalAABB.max.y ||
                b->globalAABB.max.y < reference->globalAABB.min.y)
        {
            continue;
        }

        if (b->globalAABB.min.z > reference->globalAABB.max.z ||
            b->globalAABB.max.z < reference->globalAABB.min.z)
        {
            continue;
        }            

        candidates.push_back(b);
    }
}

    // PRE-CONDITION: bodies must be sorted by aabb.min.x (ascending)
inline void queryBodyBody(Body3D* reference,
               const std::vector<Body3D*>& bodies,
               std::vector<Body3D*>& candidates) {

    candidates.clear();

    for (size_t i = 0; i < bodies.size(); i++) {
        Body3D* b = bodies[i];

        if (b == reference)
            continue;

        // b está completamente à esquerda
        if (b->worldAABB.max.x < reference->worldAABB.min.x)
            continue;

        // b está completamente à direita → pode parar (lista ordenada)
        if (b->worldAABB.min.x > reference->worldAABB.max.x)
            break;

        // checagem no eixo Y
        if (b->worldAABB.min.y > reference->worldAABB.max.y ||
            b->worldAABB.max.y < reference->worldAABB.min.y)
        {
            continue;
        }

        if (b->worldAABB.min.z > reference->worldAABB.max.z ||
            b->worldAABB.max.z < reference->worldAABB.min.z)
        {
            continue;
        }            

        candidates.push_back(b);
    }
}

inline void queryObjectBody(Object* reference,
                        const std::vector<Body3D*>& bodies,
                        std::vector<Body3D*>& candidates) {

    candidates.clear();

    for (size_t i = 0; i < bodies.size(); i++) {
        Body3D* b = bodies[i];

        // b está completamente à esquerda
        if (b->worldAABB.max.x < reference->globalAABB.min.x)
            continue;

        // b está completamente à direita → pode parar (lista ordenada)
        if (b->worldAABB.min.x > reference->globalAABB.max.x)
            break;

        // checagem no eixo Y
        if (b->worldAABB.min.y > reference->globalAABB.max.y ||
            b->worldAABB.max.y < reference->globalAABB.min.y)
        {
            continue;
        }

        if (b->worldAABB.min.z > reference->globalAABB.max.z ||
            b->worldAABB.max.z < reference->globalAABB.min.z)
        {
            continue;
        }            

        candidates.push_back(b);
    }
}
