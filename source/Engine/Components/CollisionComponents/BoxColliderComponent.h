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

/// @brief Represents an axis-aligned box collider component for entities.
struct BoxColliderComponent {

    /// @brief Size of the half box in 3D space.
    glm::vec3 halfExtents = glm::vec3(0.5f); // half-size (e.g., 1x1x1 cube)

    /// @brief Offset from the entity's origin for collider positioning.
    glm::vec3 offset = glm::vec3(0.0f);       // offset from entity transform
};


#endif //BOXCOLLIDERCOMPONENT_H
