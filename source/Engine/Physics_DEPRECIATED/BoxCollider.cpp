//
// Created by Shaun on 1/10/2024.
//

#include "BoxCollider.h"
#include "SphereCollider.h"
namespace MinPhysics
{
    BoxCollider::BoxCollider(const glm::vec3& min, const glm::vec3& max)
    : min(min), max(max) {}

    bool BoxCollider::intersects(const BoxCollider& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    bool BoxCollider::intersects(const SphereCollider& sphere) const {
        return sphere.intersects(*this); // Reuse Sphere-AABB intersection
    }
}