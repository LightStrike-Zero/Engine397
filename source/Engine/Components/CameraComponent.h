//
// Created by Shaun on 5/04/2025.
//

/**
 * @file CameraComponent.h
 * @brief Defines the CameraComponent used for viewing and projection calculations in the game engine.
 * @author Shaun
 * @date 2025-04-05
 */

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

/**
 * @struct CameraComponent
 * @brief Stores camera parameters and provides methods for view and projection matrix calculations.
 */
struct CameraComponent {
    float fov = 45.0f;                                  ///< Field of view in degrees.
    float nearPlane = 0.1f;                             ///< Near clipping plane distance.
    float farPlane = 100.0f;                            ///< Far clipping plane distance.
    
    glm::vec3 front{0.0f, 0.0f, -1.0f};        ///< Forward direction vector.
    glm::vec3 up{0.0f, 1.0f, 0.0f};            ///< Upward direction vector.
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};       ///< World's up direction.
    glm::vec3 right{1.0f, 0.0f, 0.0f};         ///< Right direction vector.
    float yaw = -90.0f;                                 ///< Horizontal rotation angle in degrees.
    float pitch = 0.0f;                                 ///< Vertical rotation angle in degrees.
    
    float movementSpeed = 10.0f;                        ///< Speed at which the camera moves.
    float mouseSensitivity = 0.1f;                      ///< Sensitivity for mouse movement.
    float rotationSpeed = 90.0f;                        ///< Speed at which the camera rotates.

    /**
     * @brief Computes and returns the view matrix based on the camera's current position and direction.
     * @param position The current position of the camera.
     * @return View matrix as glm::mat4.
     */
    glm::mat4 getViewMatrix(const glm::vec3& position) const {
        return glm::lookAt(position, position + front, up);
    }

    /**
     * @brief Computes and returns the projection matrix using perspective projection.
     * @param aspectRatio The aspect ratio of the viewport (width / height).
     * @return Projection matrix as glm::mat4.
     */
    glm::mat4 getProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};

#endif //CAMERACOMPONENT_H
