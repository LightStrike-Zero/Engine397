//
// Created by hugo on 8/04/2025.
//

#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <algorithm>  // for std::min / std::max

struct BoxColliderComponent {
    glm::vec3 halfExtents = glm::vec3(0.5f); // half-size (e.g., 1x1x1 cube)
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
