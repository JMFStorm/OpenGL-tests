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
#include "vertex_array.hpp"
#include "window.hpp"

unsigned int texture_create(const std::string& filePath, const bool isRGBA) {
    j_assert(filePath.empty() == false, "FilePath missing, cannot init texture");
    unsigned int textureId;
    int rbgMode = isRGBA ? GL_RGBA : GL_RGB;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = image_load(filePath.c_str(), &width, &height, &nrChannels);
    j_assert(data, "Failed to load texture: " + filePath);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rbgMode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    image_free_data(data);
    return textureId;
}

void texture_bind(unsigned int textureId) {
    j_assert(textureId != 0, "Id missing, cannot bind texture");
    glBindTexture(GL_TEXTURE_2D, textureId);
}

// Becnhmark vars
static float textRenderTime = 0.0f;
static int currentSecond = 0;

static FreeTypeFont gDebugFTFont = {};
static FpsCounter fpsCounter = {};

int game_run() {
    int result;
    result = glfwInit();
    j_assert(result == GLFW_TRUE, "glfwInit() failed");
    GLFWwindow* window = window_create(false);
    result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    j_assert(result == 1, "Failed to initialize OpenGL context GLAD");
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &result);
    std::cout << "Using OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";
    std::cout << "Maximum nr of vertex attributes supported: " << result << "\n";
    font_freetype_load("fonts/Roboto-Regular.ttf", &gDebugFTFont);
    unsigned int texture1 = texture_create("./images/awesomeface.png", true);
    // indicies of two triangles
    std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };
    std::vector<float> vertices {
        // positions         // texture coords
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top left 
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f  // bottom left
    };
    unsigned int vertexArrayObject = vertex_array_create();
    vertex_array_bind(vertexArrayObject);
    buffer_vertex_create(vertices);
    buffer_index_create(indices);
    vertex_array_set_attribute_pointer(0, 3, ShaderDataType::Float, false, 5 * sizeof(float), (void*)0);
    vertex_array_set_attribute_pointer(1, 2,ShaderDataType::Float, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vertex_array_unbind();
    unsigned int shader1 = shader_create("./shaders/default_vertex_shader.shader", "./shaders/default_fragment_shader.shader");
    shader_use(shader1);
    shader_set_int(shader1, "texture1", 0);
    shader_use(0);
    std::string displayTextRender = "";
    while (!window_set_should_close(window)) {
        window_handle_input_events(window);
        window_clear_screen_buffer(0.2f, 0.3f, 0.3f, 1.0f);
        texture_bind(texture1);
        shader_use(shader1);
        glm::mat4 trans; float rotateScale; { // Translations
            trans = glm::mat4(1.0f);
            double time = glfwGetTime();
            rotateScale = (float)time * 40;
            trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(rotateScale), glm::vec3(0.0f, 0.0f, 1.0f));
            trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
            shader_set_mat4(shader1, "transform", trans);
        }
        glActiveTexture(GL_TEXTURE0);
        vertex_array_bind(vertexArrayObject);
        int elementsCount = (int)indices.size();
        glDrawElements(GL_TRIANGLES, elementsCount, GL_UNSIGNED_INT, 0);
        shader_use(0);
        std::stringstream stream; std::string displayDebug1; std::string displayCurrent; std::string displayFps; { // Benchmark fps
            stream << std::fixed << std::setprecision(3) << rotateScale;
            displayDebug1 = "Rotate scale: " + stream.str();
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fpsCounter.currentTime;
            displayCurrent = "Time: " + stream.str() + "s";
            stream.str("");
            stream << std::fixed << std::setprecision(1) << fpsCounter.displayFps;
            displayFps = "FPS: " + stream.str();
            if ((int)fpsCounter.currentTime != currentSecond) {
                currentSecond = (int)fpsCounter.currentTime;
                stream.str("");
                stream << std::fixed << std::setprecision(2) << textRenderTime;
                displayTextRender = "Text render: " + stream.str() + "ms";
            } 
        }
        font_freetype_render(&gDebugFTFont, displayDebug1, 30.0f, 1160.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
        font_freetype_render(&gDebugFTFont, displayFps, 1350.0f, 1160.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
        font_freetype_render(&gDebugFTFont, displayCurrent, 1350.0f, 1130.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
        font_freetype_render(&gDebugFTFont, displayTextRender, 30.0f, 1130.0f, 1.0f, glm::vec3(0.8, 0.8f, 0.8f));
        window_swap_screen_buffer(window);
        fps_frames_increment(&fpsCounter, 1);
        fps_deltatime_calculate(&fpsCounter);
        fps_scuffed_calculate(&fpsCounter);
    }
    return 0;
}


int main() {
    int code = 0;
    std::setlocale(LC_ALL, "utf-8");
	code = game_run();
	return code;
}
