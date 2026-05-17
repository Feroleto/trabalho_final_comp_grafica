#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "Body3D.h"

struct CollisionManifold {
    bool colliding;
    float penetration;
    glm::vec3 normal;

    void reset() {
        colliding = false;
        penetration = 1e30f;
        normal = { 0, 0, 0 };
    }
};

inline void project(const Body3D& body, const glm::vec3& axis, float& min, float& max) {
    min = max = dot(body.worldCorners[0], axis);
        
    for (int i = 1; i < 8; i++) {
        float p = dot(body.worldCorners[i], axis);
        if (p < min) min = p;
        if (p > max) max = p;
    }
}

inline bool testsat(Body3D& a, Body3D& b, CollisionManifold& m) {
    m.reset();

    glm::vec3 testAxes[15];
    int axisCount = 0;

    //A
    testAxes[axisCount++] = a.worldAxes[0];
    testAxes[axisCount++] = a.worldAxes[1];
    testAxes[axisCount++] = a.worldAxes[2];

    //B
    testAxes[axisCount++] = b.worldAxes[0];
    testAxes[axisCount++] = b.worldAxes[1];
    testAxes[axisCount++] = b.worldAxes[2];

    // 9 Cross Products (A.eixo[i] x B.eixo[j])
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glm::vec3 axis = cross(a.worldAxes[i], b.worldAxes[j]);
                
            // Soh adiciona se o eixo não for nulo, ou seja, eixos nao paralelos
            if (axis.x * axis.x + axis.y * axis.y + axis.z * axis.z > 0.0001f) {
                testAxes[axisCount++] = normalize(axis);
            }
        }
    }

    //testa cada eixo
    for (int i = 0; i < axisCount; i++) {
        float minA, maxA, minB, maxB;
            
        project(a, testAxes[i], minA, maxA);
        project(b, testAxes[i], minB, maxB);

        //verifica se ha separação (Gap)
        //se minB > maxA ou minA > maxB, nao ha colisao
        float overlap = std::min(maxA, maxB) - std::max(minA, minB);

        if (overlap <= 0) {
            return false; //acha um eixo de separaçao, nao colide
        }

        //menor penetracao vai para o manifold
        if (overlap < m.penetration) {
            m.penetration = overlap;
            m.normal = testAxes[i];
        }
    }

    m.colliding = true;

        // Garantir que a normal aponte de A para B
        // Usamos a diferença de posições globais (que você já tem no Body3D)
        // Como o Body3D não guarda a matriz final, o ideal é passar ela ou 
        // recalcular a pos global aqui apenas para o sinal
    glm::vec3 posA = transformPoint(a.localTransform.modelMatrix, {0,0,0}); // Exemplo simplificado
        // O ideal é a.worldCorners[0..7] média ou passar a pos global
        
        // Melhor forma: usar o centro geométrico (média dos corners)
    glm::vec3 centerA = {0,0,0}, centerB = {0,0,0};
    for(int i=0; i<8; i++) { centerA = centerA + a.worldCorners[i]; centerB = centerB + b.worldCorners[i]; }
    centerA = centerA * 0.125f; centerB = centerB * 0.125f;

    if (dot(centerB - centerA, m.normal) < 0) {
        m.normal = m.normal * -1.0f;
    }

    return true;
}