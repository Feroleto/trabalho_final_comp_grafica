#include "camera.h"
#include "../gameLogic/entities/Object.h"
#include "../gameLogic/collision_system/utilStructures.h"

 glm::vec3 LookAtCamera::getViewDirection() {
        return normalize(lookat - position);
    }

    glm::mat4 LookAtCamera::getViewMatrix() {
        glm::vec3 zaxis = normalize(position - lookat);//inverso
        glm::vec3 xaxis = normalize(cross(up, zaxis));
        glm::vec3 yaxis = cross(zaxis, xaxis);

        glm::mat4 V(1.0f);

        V[0][0] = xaxis.x; V[1][0] = xaxis.y; V[2][0] = xaxis.z;
        V[0][1] = yaxis.x; V[1][1] = yaxis.y; V[2][1] = yaxis.z;
        V[0][2] = zaxis.x; V[1][2] = zaxis.y; V[2][2] = zaxis.z;

        V[3][0] = -dot(xaxis, position);
        V[3][1] = -dot(yaxis, position);
        V[3][2] = -dot(zaxis, position);

        return V;
    }

    glm::mat4 LookAtCamera::getProjectionMatrix() {
        float tanHalfFov =
            tan((fov * PI / 180.0f) * 0.5f);

        glm::mat4 P(0.0f);

        P[0][0] = 1.0f / (aspectRatio * tanHalfFov);
        P[1][1] = 1.0f / tanHalfFov;

        P[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);

        P[2][3] = -1.0f;

        P[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

        return P;
    }

    void LookAtCamera::update(float delta, const Object& obj1, const Object& obj2) {
        lookat = (obj1.transform.position + obj2.transform.position) * 0.5f;

        glm::vec3 obj2toObj1 = normalize(obj1.transform.position - obj2.transform.position);

        glm::vec3 desiredSide = normalize(cross(glm::vec3(0,1,0), obj2toObj1));

        if(dot(currentSide, desiredSide) < 0.0f)
        {
            desiredSide = -desiredSide;
        }

        currentSide = desiredSide;

        distance = 5.0f + sqrt(((obj1.transform.position.x - obj2.transform.position.x) * (obj1.transform.position.x - obj2.transform.position.x) + 
                   (obj1.transform.position.z - obj2.transform.position.z) * (obj1.transform.position.z - obj2.transform.position.z))) * 1.1f;

        glm::vec3 goalPosition = lookat + (currentSide * distance);
        position += (goalPosition - position) * 0.9f * delta;
        position.y = height;


    }

    glm::mat4 FreeCamera::getViewMatrix() {
        glm::vec3 target = transform.position + forward;
        glm::vec3 zaxis = normalize(transform.position - target);
        glm::vec3 xaxis = normalize(cross(up, zaxis));
        glm::vec3 yaxis = cross(zaxis, xaxis);

        glm::mat4 V(1.0f);
        V[0][0] = xaxis.x; V[1][0] = xaxis.y; V[2][0] = xaxis.z;
        V[0][1] = yaxis.x; V[1][1] = yaxis.y; V[2][1] = yaxis.z;
        V[0][2] = zaxis.x; V[1][2] = zaxis.y; V[2][2] = zaxis.z;

        V[3][0] = -dot(xaxis, transform.position);
        V[3][1] = -dot(yaxis, transform.position);
        V[3][2] = -dot(zaxis, transform.position);

        return V;
    }

    glm::mat4 FreeCamera::getProjectionMatrix() {
        float tanHalfFov =
            tan((fov * PI / 180.0f) * 0.5f);

        glm::mat4 P(0.0f);

        P[0][0] = 1.0f / (aspectRatio * tanHalfFov);
        P[1][1] = 1.0f / tanHalfFov;

        P[2][2] =
            -(farPlane + nearPlane) /
            (farPlane - nearPlane);

        P[2][3] = -1.0f;

        P[3][2] =
            -(2.0f * farPlane * nearPlane) /
            (farPlane - nearPlane);

        return P;
    }

    void FreeCamera::update(float delta) {
        float maxPitch = PI * 0.5f - 0.01f;
        if (transform.rotation.x > maxPitch) transform.rotation.x = maxPitch;
        if (transform.rotation.x < -maxPitch) transform.rotation.x = -maxPitch;

        float cy = cos(transform.rotation.y);
        float sy = sin(transform.rotation.y);
        float cp = cos(transform.rotation.x);
        float sp = sin(transform.rotation.x);

        forward.x = cp * sy;
        forward.y = sp;
        forward.z = -cp * cy;
        forward = normalize(forward);
    }