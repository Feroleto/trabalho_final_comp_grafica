#pragma once
#include "../entities/player/Player.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"

class CollisionSystem {
public:
    Object* player;
    Object* swordHitbox;

    std::vector<Object*>* objects;
    Object* enemyAttacks;

    std::vector<Object*> objectCandidates;
    std::vector<Body3D*> bodyCandidates;

    CollisionManifold CollisionManifold;


    CollisionSystem(Object* player,
                    std::vector<Object*>* objects,
                    Object* swordHitbox,
                    Object* enemyAttacks)
    {
        this->player = player;
        this->objects = objects;
        this->swordHitbox = swordHitbox;
        this->enemyAttacks = enemyAttacks;

        objectCandidates.reserve(32);
        bodyCandidates.reserve(32);
    }

    int update();

    void insertionSort(std::vector<Object*>& objects);
    
    void insertionSort(std::vector<Body3D*>& bodies);
};