#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window_create(const bool fullscreen);
void window_clear_screen_buffer(const float red, const float green, const float blue, const float alpha);
void window_swap_screen_buffer(GLFWwindow* window);
void window_mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_error_callback(int errorCode, const char* description);
void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_handle_input_events(GLFWwindow* window);
bool window_set_should_close(GLFWwindow* window);