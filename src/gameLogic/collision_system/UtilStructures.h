#pragma once
#include <cmath>
#include <algorithm>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

inline glm::vec3 transformPoint(const glm::mat4& m, const glm::vec3& v) {
    return {
        m[0][0] * v.x + m[1][0] * v.y + m[2][0]  * v.z + m[3][0],
        m[0][1] * v.x + m[1][1] * v.y + m[2][1]  * v.z + m[3][1],
        m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]
    };
}

inline glm::vec3 normalize(const glm::vec3& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0) {
        return {v.x / len, v.y / len, v.z / len};
    }
    return {0, 0, 0};//Evita divisao por zero
}

inline float dot(const glm::vec3& a, const glm::vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline glm::vec3 cross(const glm::vec3& a, const glm::vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    void update(const std::vector<glm::vec3>& verts) {
        if (verts.empty()) return;
        min = max = verts[0];
        for (const auto& v : verts) {
            min.x = std::min(min.x, v.x);
            min.y = std::min(min.y, v.y);
            min.z = std::min(min.z, v.z);
            max.x = std::max(max.x, v.x);
            max.y = std::max(max.y, v.y);
            max.z = std::max(max.z, v.z);
        }
    }

    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
};

struct Transform3D {
    glm::vec3 position {0,0,0};
    glm::vec3 rotation {0,0,0}; // radianos
    glm::vec3 scale    {1,1,1};

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    bool dirty = true;

    void updateMatrix() {
        //if (!dirty) return;

        // Escala
        glm::mat4 S = glm::mat4(1.0f);
        S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;

        float cx = cos(rotation.x), sx = sin(rotation.x);
        float cy = cos(rotation.y), sy = sin(rotation.y);
        float cz = cos(rotation.z), sz = sin(rotation.z);

        glm::mat4 Rx(1.0f);
        Rx[1][1] = cx;
        Rx[1][2] = sx;
        Rx[2][1] = -sx;
        Rx[2][2] = cx;

        glm::mat4 Ry(1.0f);
        Ry[0][0] = cy;
        Ry[0][2] = -sy;
        Ry[2][0] = sy;
        Ry[2][2] = cy;

        glm::mat4 Rz(1.0f);
        Rz[0][0] = cz;
        Rz[0][1] = sz;
        Rz[1][0] = -sz;
        Rz[1][1] = cz;

        // Translação
        glm::mat4 T = glm::mat4(1.0f);
        T[3][0] = position.x; T[3][1] = position.y; T[3][2] = position.z;

        // Combinação: M = T * Rz * Ry * Rx * S
        glm::mat4 R = Rz * (Ry * Rx);
        modelMatrix = T * (R * S);

        dirty = false;
    }
};