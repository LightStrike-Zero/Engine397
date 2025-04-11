//
// Created by hugo on 10/04/2025.
//

#ifndef SPHERECOLLIDERCOMPONENT_H
#define SPHERECOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct SphereColliderComponent {
    float radius = 1.f;
    glm::vec3 offset = glm::vec3(0.0f);
};

inline SphereColliderComponent generateSphereCollider(const std::vector<glm::vec3>& vertices) {
    glm::vec3 center(0.0f);
    for (const auto& v : vertices) center += v;
    center /= vertices.size(); // average position

    float maxRadiusSq = 0.0f;
    for (const auto& v : vertices)
        maxRadiusSq = std::max(maxRadiusSq, 2 * glm::length(v - center));

    float radius = std::sqrt(maxRadiusSq);
    return SphereColliderComponent{ radius, center };
}


#endif //SPHERECOLLIDERCOMPONENT_H
