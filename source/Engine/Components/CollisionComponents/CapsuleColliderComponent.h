//
// Created by hugo on 8/04/2025.
//

#ifndef CAPSULECOLLIDERCOMPONENT_H
#define CAPSULECOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct CapsuleColliderComponent {
    float radius = 0.5f;
    float height = 1.0f;                     // height excluding caps
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
