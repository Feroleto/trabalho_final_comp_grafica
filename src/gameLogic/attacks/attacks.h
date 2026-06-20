#pragma once
#include "../collision_system/Body3D.h"

class Object;

struct Attack {
    int currentFrame;
    int totalFrames;
    float damage;
    Object* hitboxes;
};

struct BezierCurve {
    glm::vec3 p0, p1, p2, p3;
};

struct Projectile {
    int currentFrame = 0;
    int totalFrames = 60;
    BezierCurve curve;
    Body3D hitbox = Body3D(1.0f, 1.0f, 1.0f);
    bool isActive = false;

};


void spawnBezierProjectiles(Object* attacker, Object* target, Projectile* proj1/* Projectile* proj2*/);

glm::vec3 calculateBezier(const BezierCurve& curve, float t);

void updateBezier(Attack* attack, Projectile* proj1/* Projectile* proj2*/);