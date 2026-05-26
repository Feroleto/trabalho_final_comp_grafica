#pragma once
#include "utilStructures.h"

struct Body3D {
    // em relação à origem do object que ela faz parte
    Transform3D localTransform;

    glm::mat4 finalMatrix;//cache da matriz final para evitar recomputação em cada teste de colisao

    //AABB das coordenadas globais,
    AABB worldAABB;

    //cache para o SAT
    glm::vec3 worldCorners[8];
    glm::vec3 worldAxes[3];

    bool isActive = true;

    Body3D(float width, float height, float depth, glm::vec3 offsetPos = {0,0,0}, glm::vec3 offsetRot = {0,0,0}) {
        localTransform.position = offsetPos;
        localTransform.rotation = offsetRot;
        localTransform.scale = {width, height, depth};
        isActive = true;
        localTransform.dirty = true;
    }

    inline void update(const glm::mat4& parentMatrix) {
        localTransform.updateMatrix();
        finalMatrix = parentMatrix * localTransform.modelMatrix;

        //eixos para o sat
        worldAxes[0] = normalize({
            finalMatrix[0][0],
            finalMatrix[0][1],
            finalMatrix[0][2]
        });

        worldAxes[1] = normalize({
            finalMatrix[1][0],
            finalMatrix[1][1],
            finalMatrix[1][2]
        });

        worldAxes[2] = normalize({
            finalMatrix[2][0],
            finalMatrix[2][1],
            finalMatrix[2][2]
        });

        // vertices do OBB para o SAT
        float hw = 0.5f;
        float hh = 0.5f;
        float hd = 0.5f;

        glm::vec3 localCorners[8] = {
            {-hw,-hh,-hd}, {hw,-hh,-hd}, {-hw,hh,-hd}, {hw,hh,-hd},
            {-hw,-hh, hd}, {hw,-hh, hd}, {-hw,hh, hd}, {hw,hh, hd}
        };

        std::vector<glm::vec3> pointsForAABB; 
        for(int i = 0; i < 8; i++) {
            worldCorners[i] = transformPoint(finalMatrix, localCorners[i]);
            pointsForAABB.push_back(worldCorners[i]);
        }

        worldAABB.update(pointsForAABB);
    }

    inline glm::vec3 getGlobalPosition() {
        return {
            finalMatrix[3][0],
            finalMatrix[3][1],
            finalMatrix[3][2]
        };
    }
};