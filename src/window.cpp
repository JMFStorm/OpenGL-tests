#include <iostream>
#include "main.hpp"
#include "j_util.hpp"
#include "window.hpp"

void window_clear_screen_buffer(const float red, const float green, const float blue, const float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_swap_screen_buffer(GLFWwindow* window) {
    glfwSwapBuffers(window);
}

void window_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
}

void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

void window_error_callback(int errorCode, const char* description) {
    std::cout << "GLFW error: " << errorCode << ", " << description << "\n";
}

void window_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* window_create(const bool fullscreen) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window;
    auto fullscreenOption = fullscreen ? monitor : NULL;
    auto widthOption = fullscreen ? mode->width : windows_width_default;
    auto heightOption = fullscreen ? mode->height : windows_height_default;
    window = glfwCreateWindow(widthOption, heightOption, "LearnOpenGL!", fullscreenOption, NULL);
    j_assert(window != NULL, "glfwCreateWindow() failed");
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(window_error_callback);
    glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
    glfwSetCursorPosCallback(window, window_mouse_callback);
    glfwSetScrollCallback(window, window_scroll_callback);
    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

bool window_set_should_close(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}

void window_handle_input_events(GLFWwindow* window) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // cameraTransform = glm::translate(cameraTransform, glm::vec3(1.0f, 0.0f, 0.0f) * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // cameraTransform = glm::translate(cameraTransform, glm::vec3(-1.0f, 0.0f, 0.0f) * cameraSpeed);
    }
}

