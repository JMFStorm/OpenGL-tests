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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)
#include "main.hpp"
#include "camera.hpp"
#include "free_type.hpp"
#include "fps_counter.hpp"
#include "j_util.hpp"
#include "shader.hpp"
#include "window.hpp"

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

static Camera camera_main = camera_init();
static FrameData fps_counter = {};
bool first_mouse = true;
float lastX = window_width_default / 2.0f;
float lastY = window_height_default / 2.0f;

void window_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse)
    {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera_look(&camera_main, xoffset, yoffset);
}

void window_handle_input_events(GLFWwindow* window) {
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_move(&camera_main, Move_direction::FORWARD, fps_counter.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_move(&camera_main, Move_direction::BACKWARD, fps_counter.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_move(&camera_main, Move_direction::LEFT, fps_counter.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_move(&camera_main, Move_direction::RIGHT, fps_counter.deltaTime);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera_zoom(&camera_main, yoffset);
}

int game_run() {
    GLFWwindow* window;
    {
        int result;
        result = glfwInit();
        j_assert(result == GLFW_TRUE, "glfwInit() failed");
        window = window_create(false);
        result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        j_assert(result == 1, "Failed to initialize OpenGL context GLAD");

        glfwSetCursorPosCallback(window, window_mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        std::cout << "Using OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &result);
        std::cout << "Maximum nr of vertex attributes supported: " << result << "\n";
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
    FreeTypeFont font_debug = {};
    font_freetype_load("fonts/Roboto-Regular.ttf", &font_debug);

    float vertices_box[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    unsigned int vertex_array_box, vertex_buffer;

    glGenVertexArrays(1, &vertex_array_box);
    glBindVertexArray(vertex_array_box);
    glGenBuffers(1, &vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), vertices_box, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int light_source_verticies;
    glGenVertexArrays(1, &light_source_verticies);
    glBindVertexArray(light_source_verticies);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    unsigned int texture_container = loadTexture("images/container2.png");
    unsigned int texture_container_specular_map = loadTexture("images/container2_specular.png");
    unsigned int shader_default = shader_create("./shaders/default_vertex_shader.shader", "./shaders/default_fragment_shader.shader");
    unsigned int shader_light_source = shader_create("./shaders/light_source_vertex_shader.shader", "./shaders/light_source_fragment_shader.shader");
    
    glm::vec3 light_source_position = glm::vec3(0.75f, 0.7f, 1.0f);

    shader_use(shader_default);
    shader_set_int(shader_default, "material.diffuse", 0);
    shader_set_int(shader_default, "material.specular", 1);
    shader_set_float(shader_default, "material.shininess", 32.0f);

    shader_set_vec3(shader_default, "dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader_set_vec3(shader_default, "dirLight.ambient", 0.2f, 0.2f, 0.2f);
    shader_set_vec3(shader_default, "dirLight.diffuse", 0.7f, 0.7f, 0.7f);
    shader_set_vec3(shader_default, "dirLight.specular", 1.0f, 1.0f, 1.0f);

    shader_set_vec3(shader_default, "pointLight.position", light_source_position);
    shader_set_vec3(shader_default, "pointLight.ambient", 0.1f, 0.1f, 0.1f);
    shader_set_vec3(shader_default, "pointLight.diffuse", 0.7f, 0.7f, 0.7f);
    shader_set_vec3(shader_default, "pointLight.specular", 1.0f, 1.0f, 1.0f);
    shader_set_float(shader_default, "pointLight.constant", 1.0f);
    shader_set_float(shader_default, "pointLight.linear", 0.35f);
    shader_set_float(shader_default, "pointLight.quadratic", 0.44f);
    shader_use(0);

    while (!window_set_should_close(window)) {
        float time_elapsed = glfwGetTime();
        float rotation_scale = 0.0f;

        window_handle_input_events(window);
        window_clear_screen_buffer(0.3f, 0.4f, 0.42f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_container);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_container_specular_map);
        glBindVertexArray(vertex_array_box);

        // Game logic
        light_source_position.z = sin(time_elapsed) - 2;

        // Draw create
        int elements = 36;
        shader_use(shader_default);
        shader_set_vec3(shader_default, "pointLight.position", light_source_position); // Move point light
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        glm::mat4 camera_view = camera_get_view_matrix(camera_main);
        glm::mat4 projection = glm::perspective(glm::radians(camera_main.Zoom), (float)window_width_default / (float)window_height_default, 0.1f, 100.0f);
        shader_set_mat4(shader_default, "model", model);
        shader_set_mat4(shader_default, "projection", projection);
        shader_set_mat4(shader_default, "view", camera_view);
        glDrawArrays(GL_TRIANGLES, 0, elements);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader_use(0);

        // Draw point light
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_source_position);
        model = glm::scale(model, glm::vec3(0.15f));
        shader_use(shader_light_source);
        shader_set_vec3(shader_light_source, "viewPos", camera_main.Position);
        shader_set_mat4(shader_light_source, "model", model);
        shader_set_mat4(shader_light_source, "projection", projection);
        shader_set_mat4(shader_light_source, "view", camera_view);
        glDrawArrays(GL_TRIANGLES, 0, elements);
        shader_use(0);
        {
            std::string text_deltatime, text_time_elapsed, text_fps, text_camera_pos, text_camera_front;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << fps_counter.deltaTime * 1000 << "ms";
            text_deltatime = "Deltatime: " + stream.str();
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fps_counter.currentTime;
            text_time_elapsed = "Time: " + stream.str() + "s";
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fps_counter.displayFps;
            text_fps = "FPS: " + stream.str();
            stream.str("");
            stream << std::fixed << std::setprecision(1) << camera_main.Position.x << "x " << camera_main.Position.y << "y " << camera_main.Position.z << "z";
            text_camera_pos = "Camera position: " + stream.str();
            stream.str("");
            stream << std::fixed << std::setprecision(1) << camera_main.Front.x << "x " << camera_main.Front.y << "y " << camera_main.Front.z << "z";
            text_camera_front = "Camera direction: " + stream.str();

            auto text_debug_color = glm::vec3(0.8, 0.8f, 0.8f);
            font_freetype_render(&font_debug, text_deltatime, 10.0f, 10.0f, 1.0f, text_debug_color);
            font_freetype_render(&font_debug, text_fps, 10.0f, 30.0f, 1.0f, text_debug_color);
            font_freetype_render(&font_debug, text_time_elapsed, 10.0f, 50.0f, 1.0f, text_debug_color);
            font_freetype_render(&font_debug, text_camera_pos, 10.0f, 90.0f, 1.0f, text_debug_color);
            font_freetype_render(&font_debug, text_camera_front, 10.0f, 110.0f, 1.0f, text_debug_color);
        }
        window_swap_screen_buffer(window);
        fps_counter.frames += 1;
        fps_deltatime_calculate(&fps_counter, time_elapsed);
        fps_scuffed_calculate(&fps_counter);
    }
    return 0;
}


int main() {
    std::setlocale(LC_ALL, "utf-8");
	return game_run();
}
