#include "attacks.h"
#include "../entities/Object.h"
#include <algorithm>

static float clampFloat(float value, float minValue, float maxValue) {
    return value < minValue ? minValue : (value > maxValue ? maxValue : value);
}

void spawnBezierProjectiles(Object* attacker, Object* target, Projectile* proj1, Projectile* proj2) {
    proj1->isActive = true;
    //proj2->isActive = true;
    proj1->currentFrame = 0;
    proj2->currentFrame = 0;

    proj1->curve.p0 = proj2->curve.p0 = attacker->transform.position;
    proj1->curve.p3 = proj2->curve.p3 = target->transform.position;

    float distance = glm::distance(proj1->curve.p0, proj1->curve.p3);
    float speed = 0.15f;
    
    int frames = static_cast<int>(distance / speed);
    proj1->totalFrames = proj2->totalFrames = std::max(1, frames);

    proj1->curve.p1 = {proj1->curve.p0.x + 0.25f * (proj1->curve.p3.x - proj1->curve.p0.x), 
        proj1->curve.p0.y + 1.0f, 
        proj1->curve.p0.z + 0.25f * (proj1->curve.p3.z - proj1->curve.p0.z)};
    
    proj1->curve.p2 = {proj1->curve.p0.x + 0.75f * (proj1->curve.p3.x - proj1->curve.p0.x), 
        proj1->curve.p0.y + 1.0f, 
        proj1->curve.p0.z + 0.75f * (proj1->curve.p3.z - proj1->curve.p0.z)};

    
    proj2->curve.p1 = {proj2->curve.p0.x + 0.25f * (proj2->curve.p3.x - proj2->curve.p0.x), 
        proj2->curve.p0.y + 3.0f, 
        proj2->curve.p0.z + 0.25f * (proj2->curve.p3.z - proj2->curve.p0.z)};
    
    proj2->curve.p2 = {proj2->curve.p0.x + 0.75f * (proj2->curve.p3.x - proj2->curve.p0.x), 
        proj2->curve.p0.y + 3.0f, 
        proj2->curve.p0.z + 0.75f * (proj2->curve.p3.z - proj2->curve.p0.z)};
}

glm::vec3 calculateBezier(const BezierCurve& curve, float t) {
    float u = 1.0f - t;
    return (u * u * u) * curve.p0 +
           (3.0f * u * u * t) * curve.p1 +
           (3.0f * u * t * t) * curve.p2 +
           (t * t * t) * curve.p3;
}

void updateBezier(Attack* attack, Projectile* proj1, Projectile* proj2) {
    if (proj1->isActive) {
        float t = (float)proj1->currentFrame / (float)proj1->totalFrames;
        t = clampFloat(t, 0.0f, 1.0f);

        proj1->hitbox.localTransform.position = calculateBezier(proj1->curve, t);
        proj1->hitbox.localTransform.dirty = true;
        proj1->hitbox.update(glm::mat4({1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f}));
        proj1->currentFrame++;

        if (proj1->currentFrame > proj1->totalFrames) {
            proj1->isActive = false;
        }
    }

    if (proj2->isActive) {
        float t = (float)proj2->currentFrame / (float)proj2->totalFrames;
        t = clampFloat(t, 0.0f, 1.0f);

        proj2->hitbox.localTransform.position = calculateBezier(proj2->curve, t);
        proj2->hitbox.localTransform.dirty = true;
        proj2->hitbox.update(glm::mat4({1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f}));
        proj2->currentFrame++;

        if (proj2->currentFrame > proj2->totalFrames) {
            proj2->isActive = false;
        }
    }
}