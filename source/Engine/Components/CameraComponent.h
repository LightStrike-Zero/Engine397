//
// Created by Shaun on 5/04/2025.
//

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

struct CameraComponent {
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    
    float movementSpeed = 10.0f;
    float mouseSensitivity = 0.1f;
    float rotationSpeed = 90.0f;
    
    glm::mat4 getViewMatrix(const glm::vec3& position) const {
        return glm::lookAt(position, position + front, up);
    }
    
    glm::mat4 getProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};

#endif //CAMERACOMPONENT_H
