#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "camera.hpp"
#include <iostream>

const float camera_max_fov = 75.0f;
const float camera_min_zoom = 15.0f;

Camera camera_init()
{
    Camera camera = {};
    camera.Zoom = camera_max_fov;
    camera.MouseSensitivity = 0.1f;
    camera.MovementSpeed = 4.5f;
    camera.Yaw = -90.0f;
    camera.Pitch = 0.0f;
    camera.Front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    return camera;
}

glm::mat4 camera_get_view_matrix(Camera camera)
{
    return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
}

void camera_move(Camera* camera, Move_direction direction, float deltaTime)
{
    float velocity = camera->MovementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        camera->Position += camera->Front * velocity;
    }
    if (direction == BACKWARD)
    {
        camera->Position -= camera->Front * velocity;
    }
    if (direction == LEFT)
    {
        camera->Position -= camera->Right * velocity;
    }
    if (direction == RIGHT)
    {
        camera->Position += camera->Right * velocity;
    }
}

void camera_look(Camera* camera, float xoffset, float yoffset)
{
    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;
    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (camera->Pitch > 89.0f) {
        camera->Pitch = 89.0f;
    }
    if (camera->Pitch < -89.0f) {
        camera->Pitch = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(camera->Yaw)) * cos(glm::radians(camera->Pitch));
    front.y = sin(glm::radians(camera->Pitch));
    front.z = sin(glm::radians(camera->Yaw)) * cos(glm::radians(camera->Pitch));

    camera->Front = glm::normalize(front);
    camera->Right = glm::normalize(glm::cross(camera->Front, WorldUp));
    camera->Up = glm::normalize(glm::cross(camera->Right, camera->Front));
}

void camera_zoom(Camera* camera, float yoffset)
{
    camera->Zoom -= (float)yoffset * 4;

    if (camera->Zoom < camera_min_zoom) {
        camera->Zoom = camera_min_zoom;
    }
    if (camera->Zoom > camera_max_fov) {
        camera->Zoom = camera_max_fov;
    }
}
