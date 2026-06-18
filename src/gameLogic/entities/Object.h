#ifndef GAMELOGIC_ENTITIES_OBJECT_H
#define GAMELOGIC_ENTITIES_OBJECT_H
#pragma once

#include <vector>
#include "../collision_system/Body3D.h"
#include "../../character/animatedModel.h"

class Object {
public:
    Transform3D transform;
    std::vector<Body3D> bodies;

    AABB globalAABB;
    Object() : animatedModel(nullptr), modelScale(0.01f) {}

    Object(AnimatedModel* model, float scale = 0.01f) : animatedModel(nullptr), modelScale(scale) {
        initFromModel(model, scale);
    }

    // inicializa os bodies a partir do modelo animado (tamanhos/offsets padrão)
    void initFromModel(AnimatedModel* model, float scale = 0.01f) {
        if (!model) return;
        animatedModel = model;
        modelScale = scale;
        transform.scale = {scale, scale, scale};
        auto bones = model->getBoneNames();

        for (const auto& bn : bones) {
            if (bn.empty()) continue;
            
            // ignorar dedos
            if (bn.find("HandIndex") != std::string::npos || bn.find("ToeBase") != std::string::npos) {
                continue;
            }

            // tronco
            if (bn.find("Hips") != std::string::npos) {
                addBody(Body3D(0.2f, 0.2f, 0.30f, bn, model, {0.50f, 0.1f, 0.0f}));
            }
            else if (bn.find("Spine2") != std::string::npos) {
                addBody(Body3D(0.2f, 0.7f, 0.3f, bn, model, {0.4f, 0.4f, 0.0f}));
            }
            /*else if (bn.find("Spine1") != std::string::npos) {
                addBody(Body3D(0.42f, 0.35f, 0.28f, bn, model, {0.0f, 0.18f, 0.0f}));
            }
            else if (bn.find("Spine") != std::string::npos) {
                addBody(Body3D(0.42f, 0.40f, 0.28f, bn, model, {0.0f, 0.20f, 0.0f}));
            }
            else if (bn.find("Neck") != std::string::npos) {
                addBody(Body3D(0.20f, 0.20f, 0.20f, bn, model, {0.0f, 0.10f, 0.0f}));
            }*/
            else if (bn.find("Head") != std::string::npos) {
                addBody(Body3D(0.28f, 0.32f, 0.28f, bn, model, {0.4f, 1.0f, 0.1f}));
            }
            // braço direito
            /*else if (bn.find("RightShoulder") != std::string::npos) {
                addBody(Body3D(0.18f, 0.18f, 0.18f, bn, model, {0.0f, 0.0f, 0.0f}));
            }*/
            else if (bn.find("RightArm") != std::string::npos) {
                addBody(Body3D(0.1f, 0.1f, 0.4f, bn, model, {0.5f, 0.8f, -0.5f}));
            }
            else if (bn.find("RightForeArm") != std::string::npos) {
                addBody(Body3D(0.1f, 0.1f, 0.4f, bn, model, {0.5f, 0.8f, -0.8f}));
            }
            /*else if (bn.find("RightHand") != std::string::npos && bn.find("HandIndex") == std::string::npos) {
                // corpo para a espada: caixa longa e fina vinculada ao mesmo bone
                // offset Z negativo posiciona a caixa à frente da mão no espaço do bone
                addBody(Body3D(0.05f, 0.05f, 1.20f, bn, model, {0.0f, 0.0f, -0.60f}, {0.0f, 0.0f, 0.0f}));
            }*/
            // braço esquerdo
            /*else if (bn.find("LeftShoulder") != std::string::npos) {
                addBody(Body3D(0.18f, 0.18f, 0.4f, bn, model, {0.5f, 1.0f, -0.5f}));
            }*/
            else if (bn.find("LeftArm") != std::string::npos && bn.find("ForeArm") == std::string::npos) {
                addBody(Body3D(0.1f, 0.1f, 0.4f, bn, model, {0.5f, 0.8f, 0.6f}));
            }
            else if (bn.find("LeftForeArm") != std::string::npos) {
                addBody(Body3D(0.1f, 0.1f, 0.4f, bn, model, {0.5f, 0.7f, 1.0f}));
            }
            /*else if (bn.find("LeftHand") != std::string::npos && bn.find("HandIndex") == std::string::npos) {
                addBody(Body3D(0.16f, 0.18f, 0.16f, bn, model, {0.0f, 0.0f, 0.0f}));
            }*/
            // perna direita
            else if (bn.find("RightUpLeg") != std::string::npos) {
                addBody(Body3D(0.22f, 0.50f, 0.22f, bn, model, {0.45f, -0.25f, 0.0f}));
            }
            else if (bn.find("RightLeg") != std::string::npos) {
                addBody(Body3D(0.15f, 0.8f, 0.15f, bn, model, {0.45f, -0.67f, 0.0f}));
            }
            /*else if (bn.find("RightFoot") != std::string::npos) {
                addBody(Body3D(0.20f, 0.16f, 0.40f, bn, model, {0.0f, -0.08f, 0.08f}));
            }*/
            // perna esquerda
            else if (bn.find("LeftUpLeg") != std::string::npos) {
                addBody(Body3D(0.22f, 0.50f, 0.22f, bn, model, {0.45f, -0.25f, 0.2f}));
            }
            else if (bn.find("LeftLeg") != std::string::npos) {
                addBody(Body3D(0.15f, 0.8f, 0.15f, bn, model, {0.45f, -0.67f, 0.2f}));
            }
            /*else if (bn.find("LeftFoot") != std::string::npos) {
                addBody(Body3D(0.20f, 0.16f, 0.40f, bn, model, {0.0f, -0.08f, 0.08f}));
            }*/
        }

        transform.dirty = true;
    }

    void addBody(const Body3D& body) {
        bodies.push_back(body);
    }

    void update() {
        // O Object deve usar sua própria transform de mundo para posicionar os bodies.
        // O AnimatedModel passa os offsets de bones em espaço de modelo, então o
        // movimento global do personagem vem da transformação do Object.
        transform.updateMatrix();

        for(auto& body : bodies) {
            body.update(transform.modelMatrix);
        }

        calculateGlobalAABB();
    }

    // permite ligar/desligar referencia ao modelo
    void setAnimatedModel(AnimatedModel* model, float scale = 0.01f) {
        initFromModel(model, scale);
    }

private:
    // opcional referencia ao modelo animado associado a este objeto
    AnimatedModel* animatedModel = nullptr;
    float modelScale = 0.01f;

    void calculateGlobalAABB() {
        if (bodies.empty()) return;

        this->globalAABB.min = this->bodies[0].worldAABB.min;
        this->globalAABB.max = this->bodies[0].worldAABB.max;

        for(const auto& body : this->bodies) {
            this->globalAABB.min.x = std::min(body.worldAABB.min.x, this->globalAABB.min.x);
            this->globalAABB.min.y = std::min(body.worldAABB.min.y, this->globalAABB.min.y);
            this->globalAABB.min.z = std::min(body.worldAABB.min.z, this->globalAABB.min.z);
            this->globalAABB.max.x = std::max(body.worldAABB.max.x, this->globalAABB.max.x);
            this->globalAABB.max.y = std::max(body.worldAABB.max.y, this->globalAABB.max.y);
            this->globalAABB.max.z = std::max(body.worldAABB.max.z, this->globalAABB.max.z);
        }
    }
};
#endif // GAMELOGIC_ENTITIES_OBJECT_H