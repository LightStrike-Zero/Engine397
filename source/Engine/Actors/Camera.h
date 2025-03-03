/**
 * @file Camera.h
 * @author Shaun
 * @date 13/09/2024
 * @modfied 6/10/2024
 * @brief Declaration of the Camera class responsible for managing camera behavior and collision detection.
 */

#ifndef CAMERA_H
#define CAMERA_H

// =============================
// Third-Party Library Includes
// =============================
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// =============================
// Project-Specific Includes
// =============================
#include <string>
#include <vector>


class Event;

/**
 * @class Camera
 * @brief Manages camera positioning, orientation, movement, and collision detection.
 *
 * The Camera class handles the camera's position and orientation in the 3D environment.
 * It also includes a spherical collider to detect collisions with objects in the scene.
 * The class provides methods to update the camera's state based on user input and time,
 * retrieve the view and projection matrices for rendering, and manipulate the camera's position.
 */
class Camera
{
public:
    /**
     * @brief Constructs a Camera with specified parameters.
     *
     * Initializes the camera's position, target, up vector, movement speed, and collider radius.
     * Also initializes projection parameters.
     *
     * @param position The initial position of the camera.
     * @param target The point the camera is initially looking at.
     * @param up The up direction for the camera.
     * @param speed The movement speed of the camera.
     * @param colliderRadius The radius of the camera's spherical collider.
     * @param fov The field of view angle in degrees.
     * @param aspectRatio The aspect ratio of the viewport (width/height).
     * @param nearPlane The near clipping plane distance.
     * @param farPlane The far clipping plane distance.
     */
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float speed, float colliderRadius,
           float fov, float aspectRatio, float nearPlane, float farPlane);

 bool isMoving() const;
 bool isSprinting() const;
    /**
     * @brief Updates the camera's position and orientation based on input and time.
     *
     * Processes movement offsets, yaw and pitch adjustments, and updates the camera's vectors.
     *
     * @param deltaTime The time elapsed since the last update.
     * @param yawOffset The change in yaw (rotation around the Y-axis).
     * @param pitchOffset The change in pitch (rotation around the X-axis).
     * @param movementOffset The movement direction and magnitude.
     */
    void update(float deltaTime, float yawOffset, float pitchOffset, glm::vec3 movementOffset);

    /**
     * @brief Retrieves the view matrix representing the camera's current orientation and position.
     *
     * @return A 4x4 view matrix.
     */
    glm::mat4 getViewMatrix() const;

    /**
     * @brief Retrieves the projection matrix based on the camera's projection parameters.
     *
     * @return A 4x4 projection matrix.
     */
    glm::mat4 getProjectionMatrix() const;

    /**
     * @brief Sets the projection parameters and updates the projection matrix.
     *
     * @param fov The field of view angle in degrees.
     * @param aspectRatio The aspect ratio of the viewport (width/height).
     * @param nearPlane The near clipping plane distance.
     * @param farPlane The far clipping plane distance.
     */
    void setProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane);

    /**
     * @brief Retrieves the camera's current position in the world.
     *
     * @return A 3D vector representing the camera's position.
     */
    glm::vec3 getPosition() const;


    /**
     * @brief Sets the camera's position to a new value.
     *
     * @param position The new position for the camera.
     */
    void setPosition(const glm::vec3& position);

    /**
     * @brief Adjusts the camera's position by a specified offset.
     *
     * @param offsetPosition The vector by which to offset the camera's current position.
     */
    void addToPosition(const glm::vec3& offsetPosition);

    const float getMovementSpeed() const {return m_speed;}

    const glm::vec3& getVelocity() const { return m_cameraVelocity; }
    void setVelocity(const glm::vec3& velocity) { m_cameraVelocity = velocity; }

    const glm::vec3& getAcceleration() const { return m_cameraAcceleration; }
    void setAcceleration(const glm::vec3& acceleration) { m_cameraAcceleration = acceleration; }

    bool isGrounded() const { return m_isCameraGrounded; }
    void setGrounded(bool grounded) { m_isCameraGrounded = grounded; }

    void setFOV(float fov) { m_fov = fov; }

    void setSpeed(float speed) { m_speed = speed; }

    void processKeyboard(const std::string& direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch);
    void setAspectRatio(float aspectRatio);

   private:
    // Camera Attributes
    glm::vec3 m_position;   /**< Current position of the camera in the world */
    glm::vec3 m_previousPosition; // Position in the previous frame
    glm::vec3 m_front;      /**< Direction the camera is facing */
    glm::vec3 m_up;         /**< Up direction relative to the camera */
    glm::vec3 m_right;      /**< Right direction relative to the camera */
    glm::vec3 m_worldUp;    /**< World's up direction */

    // Euler Angles
    float m_yaw;            /**< Yaw angle (rotation around the Y-axis) */
    float m_pitch;          /**< Pitch angle (rotation around the X-axis) */

    // Camera options
    float m_speed;          /**< Movement speed of the camera */
    float m_sensitivity = 0.1f; // Default mouse sensitivity

    // Projection parameters
    float m_fov;            /**< Field of view angle in degrees */
    float m_aspectRatio;    /**< Aspect ratio (width/height) */
    float m_nearPlane;      /**< Near clipping plane distance */
    float m_farPlane;       /**< Far clipping plane distance */

    /// Physics properties for the camera
    glm::vec3 m_cameraVelocity = glm::vec3(0.0f);
    bool m_isCameraGrounded = false;
    glm::vec3 m_cameraAcceleration = glm::vec3(0.0f);


    /**
     * @brief Updates the camera's directional vectors based on the current yaw and pitch.
     *
     * Recalculates the front, right, and up vectors to ensure the camera's orientation is correct.
     */
    void updateCameraVectors();

};

#endif // CAMERA_H
