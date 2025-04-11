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

/// @brief Represents a capsule-shaped collider, useful for player characters.
struct CapsuleColliderComponent {

    /// @brief Radius of the capsule's hemispherical ends.
    float radius = 0.5f;

    /// @brief Height of the cylindrical portion of the capsule (excluding caps).
    float height = 1.0f;

    /// @brief Offset from the entity's origin for collider positioning.
    glm::vec3 offset = glm::vec3(0.0f);
};

#endif //CAPSULECOLLIDERCOMPONENT_H
