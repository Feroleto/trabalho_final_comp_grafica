#ifndef CAMERA_CAMERA_H
#define CAMERA_CAMERA_H
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "../gameLogic/entities/Object.h"

constexpr float PI = 3.14159265359f;

struct FreeCamera {
    Transform3D transform;

    float nearPlane = 0.1f; // positivo
    float farPlane = 100.0f; // positivo

    float fov = 60.0f;
    float aspectRatio = 4.0f / 3.0f;

    float yaw = 0.0f;
    float pitch = 0.0f;
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    void update(float delta);
};


struct LookAtCamera {
    glm::vec3 position = glm::vec3(0.0f,0.0f,5.0f);
    glm::vec3 lookat= glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 view = lookat - position;
    glm::vec3 up  = glm::vec3(0.0f,1.0f,0.0f);

    float nearPlane = 0.1f;//positivo
    float farPlane = 100.0f;//positivo

    float fov = 50.0f;
    float aspectRatio = 4.0f / 3.0f;

    glm::vec3 currentSide = {1,0,0};
    float distance = 2.0f;
    float height = 1.0f;

    glm::vec3 getViewDirection();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    void update(float delta, const Object& obj1, const Object& obj2);
};

#endif//CAMERA_CAMERA_H