//
// Created by hugo on 8/04/2025.
//

/**
 * @file BoxColliderComponent.h
 * @brief Defines the the struct that represents an axis-aligned box collider component for entities.
 * @author Hugo
 * @date 2025-04-08
 */

#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <algorithm>  // for std::min / std::max

/// @brief Represents an axis-aligned box collider component for entities.
struct BoxColliderComponent {

    /// @brief Size of the half box in 3D space.
    glm::vec3 halfExtents = glm::vec3(0.5f); // half-size (e.g., 1x1x1 cube)

    /// @brief Offset from the entity's origin for collider positioning.
    glm::vec3 offset = glm::vec3(0.0f);       // offset from entity transform
};

inline BoxColliderComponent generateBoxCollider(const std::vector<glm::vec3>& vertices) {
    glm::vec3 min = vertices[0];
    glm::vec3 max = vertices[0];

    for (const auto& v : vertices) {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    glm::vec3 size = max - min;
    glm::vec3 offset = (min + max) * 0.5f;

    return BoxColliderComponent{ size, offset };
}


#endif //BOXCOLLIDERCOMPONENT_H
