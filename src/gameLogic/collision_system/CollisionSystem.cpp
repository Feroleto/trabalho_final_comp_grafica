#include "CollisionSystem.h"

// Atualiza a posição global do personagem no main.cpp quando o sistema de colisão corrige o player.
extern float g_CharacterX;
extern float g_CharacterY;
extern float g_CharacterZ;

void CollisionSystem::update() {
    insertionSort(*objects);
    //insertionSort(*enemyAttacks);

    CollisionManifold.reset();
    

    queryObjectObject(player, *objects, objectCandidates);

    // acumular correcoes para o player e aplicar ao final
    glm::vec3 accumulatedPlayerDisplacement = {0.0f, 0.0f, 0.0f};

    for (auto& obj : objectCandidates) {
        for (auto& body : obj->bodies) {
            // testar contra todos os corpos do player
            for (auto& pbody : player->bodies) {
                if (testsat(pbody, body, CollisionManifold)) {
                    
                    glm::vec3 mtv = CollisionManifold.normal * CollisionManifold.penetration;

                    // mover o player na direcao oposta
                    accumulatedPlayerDisplacement -= mtv;
                }
            }
        }
    }

    if (accumulatedPlayerDisplacement.x != 0.0f || accumulatedPlayerDisplacement.z != 0.0f) {
        accumulatedPlayerDisplacement.y = 0.0f;//nao mexe na altura
        player->transform.position += accumulatedPlayerDisplacement;
        player->transform.dirty = true;

        // Atualiza as variáveis globais de personagem em main.cpp
        g_CharacterX = player->transform.position.x;
        g_CharacterY = player->transform.position.y;
        g_CharacterZ = player->transform.position.z;

        player->Object::update();
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
