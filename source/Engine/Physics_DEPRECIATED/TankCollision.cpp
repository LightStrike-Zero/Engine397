//
// Created by Hugo on 4/12/2025.
//

#include "TankCollision.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>



namespace {

    // Projects a box onto an axis and returns min/max scalar values
    void projectBoxOntoAxis(const glm::vec3& center, const glm::vec3 axes[3],
                            const glm::vec3& halfExtents, const glm::vec3& axis,
                            float& outMin, float& outMax) {
        float projectionCenter = glm::dot(center, axis);
        float projectionRadius =
            std::abs(glm::dot(axes[0], axis)) * halfExtents.x +
            std::abs(glm::dot(axes[1], axis)) * halfExtents.y +
            std::abs(glm::dot(axes[2], axis)) * halfExtents.z;

        outMin = projectionCenter - projectionRadius;
        outMax = projectionCenter + projectionRadius;
    }

    // Checks for overlap on a single axis
    bool axisOverlap(const glm::vec3& axis,
                     const glm::vec3& aCenter, const glm::vec3 aAxes[3], const glm::vec3& aExtents,
                     const glm::vec3& bCenter, const glm::vec3 bAxes[3], const glm::vec3& bExtents) {
        float minA, maxA, minB, maxB;
        projectBoxOntoAxis(aCenter, aAxes, aExtents, axis, minA, maxA);
        projectBoxOntoAxis(bCenter, bAxes, bExtents, axis, minB, maxB);

        return !(maxA < minB || maxB < minA);
    }

}

bool checkBoxtoBoxCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
                            const BoxColliderComponent& b, const TransformComponent& bTransform) {
    // Orientation axes (assume local basis vectors rotated by transform.rotation)
    glm::mat4 aModel = glm::mat4(1.0f);
    aModel = glm::rotate(aModel, glm::radians(aTransform.rotation.y), glm::vec3(0, 1, 0)); // Yaw
    aModel = glm::rotate(aModel, glm::radians(aTransform.rotation.x), glm::vec3(1, 0, 0)); // Pitch
    aModel = glm::rotate(aModel, glm::radians(aTransform.rotation.z), glm::vec3(0, 0, 1)); // Roll


    glm::mat4 bModel = glm::mat4(1.0f);
    bModel = glm::rotate(bModel, glm::radians(bTransform.rotation.y), glm::vec3(0, 1, 0)); // Yaw
    bModel = glm::rotate(bModel, glm::radians(bTransform.rotation.x), glm::vec3(1, 0, 0)); // Pitch
    bModel = glm::rotate(bModel, glm::radians(bTransform.rotation.z), glm::vec3(0, 0, 1)); // Roll


    glm::vec3 aAxes[3] = {
        glm::vec3(aModel[0]), // Right
        glm::vec3(aModel[1]), // Up
        glm::vec3(aModel[2])  // Forward
    };
    glm::vec3 bAxes[3] = {
        glm::vec3(bModel[0]),
        glm::vec3(bModel[1]),
        glm::vec3(bModel[2])
    };

    glm::vec3 aCenter = aTransform.position + a.offset;
    glm::vec3 bCenter = bTransform.position + b.offset;

    glm::vec3 toCenter = bCenter - aCenter;

    // 15 potential separating axes:
    std::array<glm::vec3, 15> axes;
    int idx = 0;

    // 3 face normals of A
    for (int i = 0; i < 3; ++i) axes[idx++] = aAxes[i];
    // 3 face normals of B
    for (int i = 0; i < 3; ++i) axes[idx++] = bAxes[i];
    // 9 cross products
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            axes[idx++] = glm::cross(aAxes[i], bAxes[j]);

    for (const auto& axis : axes) {
        if (glm::length2(axis) < 1e-6f) continue; // skip near-zero vectors
        glm::vec3 normalizedAxis = glm::normalize(axis);
        if (!axisOverlap(normalizedAxis, aCenter, aAxes, a.halfExtents, bCenter, bAxes, b.halfExtents)) {
            return false; // Found a separating axis → no collision
        }
    }

    return true; // No separating axis found → collision
}
