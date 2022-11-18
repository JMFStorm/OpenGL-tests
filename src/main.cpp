#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <stdio.h>
#include <thread>
#include <vector>
#pragma warning(pop)
#include "main.hpp"
#include "buffer.hpp"
#include "free_type.hpp"
#include "fps_counter.hpp"
#include "image.hpp"
#include "j_util.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "window.hpp"

static FrameData fps_counter = {};
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
bool first_mouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

void window_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (first_mouse) {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    float radians_yaw = glm::radians(yaw);
    float radians_pitch = glm::radians(pitch);
    float cos_yaw = cos(radians_yaw);
    float cos_pitch = cos(radians_pitch);
    float sin_yaw = sin(radians_yaw);
    float sin_pitch = sin(radians_pitch);
    glm::vec3 front = glm::vec3(cos_yaw * cos_pitch, sin_pitch, sin_yaw * cos_pitch);
    camera_front = glm::normalize(front);
}

void window_handle_input_events(GLFWwindow* window) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = static_cast<float>(2.5 * fps_counter.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_position += cameraSpeed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_position -= cameraSpeed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
    }
}

int game_run() {
    GLFWwindow* window;
    {
        int result;
        result = glfwInit();
        j_assert(result == GLFW_TRUE, "glfwInit() failed");
        window = window_create(false);
        result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSetCursorPosCallback(window, window_mouse_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        j_assert(result == 1, "Failed to initialize OpenGL context GLAD");
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &result);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        std::cout << "Using OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";
        std::cout << "Maximum nr of vertex attributes supported: " << result << "\n";
    }
    FreeTypeFont font_debug = {};
    font_freetype_load("fonts/Roboto-Regular.ttf", &font_debug);
    unsigned int vertex_array;
    int elements_count;
    {
        std::vector<unsigned int> indices;
        std::vector<float> vertices;
        indices = { 0, 1, 2, 2, 3, 0 }; // indicies of two triangles
        elements_count = (int)indices.size();
        vertices = {
            // xy          // uv
            -0.5f,  0.5f,  0.0f, 1.0f, // top left 
             0.5f,  0.5f,  1.0f, 1.0f, // top right
             0.5f, -0.5f,  1.0f, 0.0f, // bottom right
            -0.5f, -0.5f,  0.0f, 0.0f  // bottom left
        };
        vertex_array = vertex_array_create();
        vertex_array_bind(vertex_array);
        buffer_vertex_create(vertices);
        buffer_index_create(indices);
        vertex_array_set_attribute_pointer(0, 2, ShaderDataType::Float, false, 4 * sizeof(float), (void*)0);
        vertex_array_set_attribute_pointer(1, 2, ShaderDataType::Float, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        vertex_array_unbind();
    }
    unsigned int texture_smile;
    unsigned int shader_default;
    {
        texture_smile = texture_create("./images/awesomeface.png", true);
        shader_default = shader_create("./shaders/default_vertex_shader.shader", "./shaders/default_fragment_shader.shader");
        shader_use(shader_default);
        shader_set_int(shader_default, "texture1", 0);
        shader_use(0);
    }
    while (!window_set_should_close(window)) {
        float rotation_scale;
        window_handle_input_events(window);
        window_clear_screen_buffer(0.3f, 0.4f, 0.42f, 1.0f);
        {
            shader_use(shader_default);
            glm::mat4 model; glm::mat4 projection; glm::mat4 view;
            model = glm::mat4(1.0f);
            double time = glfwGetTime();
            rotation_scale = (float)time * 40;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation_scale), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            shader_set_mat4(shader_default, "model", model);
            projection = glm::perspective(glm::radians(fov), (float)4 / (float)3, 0.1f, 100.0f);
            view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
            shader_set_mat4(shader_default, "projection", projection);
            shader_set_mat4(shader_default, "view", view);
            texture_bind(texture_smile);
            glActiveTexture(GL_TEXTURE0);
            vertex_array_bind(vertex_array);
            glDrawElements(GL_TRIANGLES, elements_count, GL_UNSIGNED_INT, 0);
            shader_use(0);
        }
        {
            std::string text_rotation_scale; std::string text_time_elapsed; std::string text_fps;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(3) << rotation_scale;
            text_rotation_scale = "Rotate scale: " + stream.str();
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fps_counter.currentTime;
            text_time_elapsed = "Time: " + stream.str() + "s";
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fps_counter.displayFps;
            text_fps = "FPS: " + stream.str();
            font_freetype_render(&font_debug, text_rotation_scale, 30.0f, 860.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
            font_freetype_render(&font_debug, text_fps, 900.0f, 860.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
            font_freetype_render(&font_debug, text_time_elapsed, 900.0f, 830.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
        }
        window_swap_screen_buffer(window);
        fps_frames_increment(&fps_counter, 1);
        fps_deltatime_calculate(&fps_counter);
        fps_scuffed_calculate(&fps_counter);
    }
    return 0;
}


int main() {
    std::setlocale(LC_ALL, "utf-8");
	return game_run();
}
