#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>	

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed = 2.5f;

    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f), float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 GetViewMatrix() const;
    void ProcessMouseMovement(float xoffset, float yoffset, float sensitivity = 0.1f);
    glm::mat4 GetProjectionMatrix(float fovDegrees, float aspect, float zNear, float zFar) const;
    void UpdateCameraVectors();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);

};