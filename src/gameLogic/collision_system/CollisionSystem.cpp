#include "CollisionSystem.h"

void CollisionSystem::update() {
    insertionSort(*objects);
    //insertionSort(*enemyAttacks);

    CollisionManifold.reset();
    

    queryObjectObject(player, *objects, objectCandidates);

    for (auto& obj : objectCandidates) {
        for (auto& body : obj->bodies) {
            testsat(player->bodies[0], body, CollisionManifold);
        }
    }

    for (size_t i = 0; i < objects->size(); i++) {
        //corrigir posições
    }
}

void CollisionSystem::insertionSort(std::vector<Object*>& objects) {
    for (size_t i = 1; i < objects.size(); i++) {
        Object* key = objects[i];
        size_t j = i;

        while (j > 0 &&
               objects[j - 1]->globalAABB.min.x > key->globalAABB.min.x) {
            objects[j] = objects[j - 1];
            j--;
        }

        objects[j] = key;
    }
}

void CollisionSystem::insertionSort(std::vector<Body3D*>& bodies) {
    for (size_t i = 1; i < bodies.size(); i++) {
        Body3D* key = bodies[i];
        size_t j = i;

        while (j > 0 &&
               bodies[j - 1]->worldAABB.min.x > key->worldAABB.min.x) {
            bodies[j] = bodies[j - 1];
            j--;
        }

        bodies[j] = key;
    }
}
