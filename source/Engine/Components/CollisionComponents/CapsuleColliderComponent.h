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
};

#endif //CAPSULECOLLIDERCOMPONENT_H
