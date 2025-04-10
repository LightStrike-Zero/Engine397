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

#endif //SPHERECOLLIDERCOMPONENT_H
