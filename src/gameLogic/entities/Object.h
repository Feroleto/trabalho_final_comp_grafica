#include <vector>
#include "../collision_system/Body3D.h"

class Object {
public:
    Transform3D transform;
    std::vector<Body3D> bodies;

    AABB globalAABB;

    void addBody(const Body3D& body) {
        bodies.push_back(body);
    }

    void update() {
        if (!transform.dirty) return;
        
        transform.updateMatrix();

        for(auto& body : bodies) {
            body.update(transform.modelMatrix); 
        }

        calculateGlobalAABB();
    }

private:

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