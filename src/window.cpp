#include "window.hpp"
#include "jUtil.hpp"
#include "main.hpp"

void ClearScreenBuffer(const float red, const float green, const float blue, const float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SwapScreenBuffer(GLFWwindow* window) {
    glfwSwapBuffers(window);
}

void MouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

void GlfwErrorCallback(int errorCode, const char* description) {
    printf("GLFW error %d: %s\n", errorCode, description);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* CreateWindow(const bool fullscreen) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window;
    auto fullscreenOption = fullscreen ? monitor : NULL;
    auto widthOption = fullscreen ? mode->width : WINDOW_WIDTH_DEFAULT;
    auto heightOption = fullscreen ? mode->height : WINDOW_HEIGHT_DEFAULT;
    window = glfwCreateWindow(widthOption, heightOption, "LearnOpenGL!", fullscreenOption, NULL);
    j_assert(window != NULL, "glfwCreateWindow() failed");
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(GlfwErrorCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

bool WindowShouldClose(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}

void HandleWindowInputEvents(GLFWwindow* window) {
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

