#include "Camera.h"

Camera::Camera(const glm::vec3& position, float yaw, float pitch)
    : position(position), yaw(yaw), pitch(pitch), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float sensitivity)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 temp_front;
    temp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    temp_front.y = sin(glm::radians(pitch));
    temp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(temp_front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}