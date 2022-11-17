#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* CreateWindow(const bool fullscreen);
void ClearScreenBuffer(const float red, const float green, const float blue, const float alpha);
void SwapScreenBuffer(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void GlfwErrorCallback(int errorCode, const char* description);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void HandleWindowInputEvents(GLFWwindow* window);
bool WindowShouldClose(GLFWwindow* window);