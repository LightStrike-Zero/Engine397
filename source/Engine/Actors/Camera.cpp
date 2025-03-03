#include "Camera.h"

#include <ostream>
#include <glm/gtc/matrix_transform.hpp>



Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float speed, float colliderRadius,
               float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_position(position), m_worldUp(up), m_speed(speed),
      m_yaw(-90.0f), m_pitch(0.0f), m_fov(fov), m_aspectRatio(aspectRatio),
      m_nearPlane(nearPlane), m_farPlane(farPlane), m_cameraVelocity(0.0f), m_cameraAcceleration(0.0f), m_isCameraGrounded(false)
{
    m_front = glm::normalize(target - position);
    updateCameraVectors();
}

bool Camera::isMoving() const
{
    return glm::length(m_position - m_previousPosition) > std::numeric_limits<float>::epsilon();

}

bool Camera::isSprinting() const
{
    if(m_speed > 15.0f)
    {
        return true;
    }else
        return false;
}

void Camera::update(float deltaTime, float yawOffset, float pitchOffset, glm::vec3 movementOffset)
{

    m_previousPosition = m_position;

    m_yaw += yawOffset;
    m_pitch += pitchOffset;

    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    updateCameraVectors();

    m_position += movementOffset.x * m_right * m_speed * deltaTime;  // left/right
    m_position += movementOffset.z * m_front * m_speed * deltaTime;  // forward/backward
    m_position += movementOffset.y * m_worldUp * m_speed * deltaTime; // up/down

    m_cameraAcceleration += movementOffset;

}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::setProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

glm::vec3 Camera::getPosition() const
{
    return m_position;
}

void Camera::setPosition(const glm::vec3& position)
{
    m_position = position;
}

void Camera::addToPosition(const glm::vec3& offsetPosition)
{
    m_position += offsetPosition;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processKeyboard(const std::string& direction, float deltaTime) {
    float velocity = m_speed * deltaTime;
    if (direction == "FORWARD")
        m_position += m_front * velocity;
    if (direction == "BACKWARD")
        m_position -= m_front * velocity;
    if (direction == "LEFT")
        m_position -= m_right * velocity;
    if (direction == "RIGHT")
        m_position += m_right * velocity;
}


void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // Constrain pitch to prevent flipping
    if (constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    // Update the camera vectors
    updateCameraVectors();
}

void Camera::setAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
}

