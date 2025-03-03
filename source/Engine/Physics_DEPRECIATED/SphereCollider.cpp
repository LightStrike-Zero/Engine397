//
// Created by Shaun on 1/10/2024.
//

#include "SphereCollider.h"
#include "BoxCollider.h"
namespace MinPhysics
{
    SphereCollider::SphereCollider(const glm::vec3& center, float radius)
    : center(center), radius(radius) {}

    bool SphereCollider::intersects(const SphereCollider& other) const {
        glm::vec3 diff = other.center - center;
        float distanceSquared = glm::dot(diff, diff);
        float radiusSum = radius + other.radius;
        return distanceSquared <= radiusSum * radiusSum;
    }

    bool SphereCollider::intersects(const BoxCollider& box) const {
        // Implement Sphere-AABB collision detection
        float sqDist = 0.0f;

        if (center.x < box.min.x) sqDist += (box.min.x - center.x) * (box.min.x - center.x);
        else if (center.x > box.max.x) sqDist += (center.x - box.max.x) * (center.x - box.max.x);

        if (center.y < box.min.y) sqDist += (box.min.y - center.y) * (box.min.y - center.y);
        else if (center.y > box.max.y) sqDist += (center.y - box.max.y) * (center.y - box.max.y);

        if (center.z < box.min.z) sqDist += (box.min.z - center.z) * (box.min.z - center.z);
        else if (center.z > box.max.z) sqDist += (center.z - box.max.z) * (center.z - box.max.z);

        return sqDist <= radius * radius;
    }
}