#pragma once
#include <glm/glm.hpp>

static glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

enum Move_direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct Camera {
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};

Camera camera_init();
glm::mat4 camera_get_view_matrix(Camera camera);
void camera_move(Camera* camera, Move_direction direction, float deltaTime);
void camera_look(Camera* camera, float xoffset, float yoffset);
void camera_zoom(Camera* camera, float yoffset);