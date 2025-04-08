//
// Created by hugo on 8/04/2025.
//

#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent {
    glm::vec3 halfExtents = glm::vec3(0.5f); // half-size (e.g., 1x1x1 cube)
    glm::vec3 offset = glm::vec3(0.0f);       // offset from entity transform
};


#endif //BOXCOLLIDERCOMPONENT_H
