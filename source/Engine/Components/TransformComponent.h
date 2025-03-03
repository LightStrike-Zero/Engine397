//
// Created by Shaun on 28/12/2024.
//

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);                                           // In degrees
    glm::vec3 scale = glm::vec3(1.0f);

    // Generate the model matrix
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);                                    // Apply position
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));   // Rotate around X-axis
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));   // Rotate around Y-axis
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));   // Rotate around Z-axis
        model = glm::scale(model, scale);                                           // Apply scale
        return model;
    }

    void setFromModelMatrix(const glm::mat4& modelMatrix) {
        position = glm::vec3(modelMatrix[3]); // Translation

        // Extract scale
        scale.x = glm::length(glm::vec3(modelMatrix[0]));
        scale.y = glm::length(glm::vec3(modelMatrix[1]));
        scale.z = glm::length(glm::vec3(modelMatrix[2]));

        // Normalize rotation basis vectors to extract rotation matrix
        glm::mat3 rotationMatrix = glm::mat3(
            glm::vec3(modelMatrix[0]) / scale.x,
            glm::vec3(modelMatrix[1]) / scale.y,
            glm::vec3(modelMatrix[2]) / scale.z
        );

        // Extract Euler angles from rotation matrix
        rotation.y = glm::degrees(asin(-rotationMatrix[2][0]));
        if (cos(glm::radians(rotation.y)) > 0.0001f) { // Avoid gimbal lock
            rotation.x = glm::degrees(atan2(rotationMatrix[2][1], rotationMatrix[2][2]));
            rotation.z = glm::degrees(atan2(rotationMatrix[1][0], rotationMatrix[0][0]));
        } else {
            rotation.x = glm::degrees(atan2(-rotationMatrix[1][2], rotationMatrix[1][1]));
            rotation.z = 0.0f;
        }
    }
};

#endif //TRANSFORMCOMPONENT_H
