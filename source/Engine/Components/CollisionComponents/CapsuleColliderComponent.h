//
// Created by hugo on 8/04/2025.
//

/**
 * @file CapsuleColliderComponent.h
 * @brief Defines the the struct that represents a capsule-shaped collider, useful for player characters.
 * @author Hugo
 * @date 2025-04-08
 */

#ifndef CAPSULECOLLIDERCOMPONENT_H
#define CAPSULECOLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <algorithm>  // for std::min / std::max

/// @brief Represents a capsule-shaped collider, useful for player characters.
struct CapsuleColliderComponent {

    /// @brief Radius of the capsule's hemispherical ends.
    float radius = 0.5f;

    /// @brief Height of the cylindrical portion of the capsule (excluding caps).
    float height = 1.0f;

    /// @brief Offset from the entity's origin for collider positioning.
    glm::vec3 offset = glm::vec3(0.0f);

    CapsuleColliderComponent generateCapsuleCollider(const std::vector<glm::vec3>& vertices);

};

inline CapsuleColliderComponent generateCapsuleCollider(const std::vector<glm::vec3>& vertices)
{
    glm::vec3 min = vertices[0];
    glm::vec3 max = vertices[0];

    for (const auto& v : vertices) {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    float height = max.y - min.y;
    float radius = 0.0f;

    // Compute max radius from vertical axis (XZ)
    glm::vec3 centerXZ = glm::vec3((min.x + max.x) * 0.5f, 0.0f, (min.z + max.z) * 0.5f);
    for (const auto& v : vertices) {
        glm::vec3 posXZ = glm::vec3(v.x, 0.0f, v.z);
        float dist = glm::length(posXZ - centerXZ);
        radius = std::max(radius, dist);
    }

    glm::vec3 offset = (min + max) * 0.5f;

    return CapsuleColliderComponent{ radius, height, offset };
}

#endif //CAPSULECOLLIDERCOMPONENT_H
