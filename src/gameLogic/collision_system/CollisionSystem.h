#pragma once
#include "../entities/player/Player.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"

class CollisionSystem {
public:
    Player* player;

    std::vector<Object*>* objects;
    std::vector<Body3D*>* enemyAttacks;

    std::vector<Object*> objectCandidates;
    std::vector<Body3D*> bodyCandidates;

    CollisionManifold CollisionManifold;


    CollisionSystem(Player* player,
                    std::vector<Object*>* objects,
                    std::vector<Body3D*>* enemyAttacks)
    {
        this->player = player;
        this->objects = objects;
        this->enemyAttacks = enemyAttacks;

        objectCandidates.reserve(32);
        bodyCandidates.reserve(32);
    }

    void update();

    void insertionSort(std::vector<Object*>& objects);
    
    void insertionSort(std::vector<Body3D*>& bodies);
};