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
#include "freeType.hpp"
#include "image.hpp"
#include "jUtil.hpp"
#include "shader.hpp"
#include "vertexArray.hpp"
#include "window.hpp"

unsigned int buffer_vertex_create(const std::vector<float> vertices) {
    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    int sizeInBytes = (int)vertices.size() * sizeof(unsigned int);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, &vertices[0], GL_STATIC_DRAW);
    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    if (sizeInBytes != size) {
        glDeleteBuffers(1, &vertexBufferObject);
        j_assert(sizeInBytes != size, "VertexBufferObject GL_BUFFER_SIZE failed");
    }
    return vertexBufferObject;
}

unsigned int buffer_index_create(const std::vector<unsigned int> indices) {
    unsigned int indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    int sizeInBytes = (int)indices.size() * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, &indices[0], GL_STATIC_DRAW);
    GLint size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    if (sizeInBytes != size) {
        glDeleteBuffers(1, &indexBufferObject);
        j_assert(sizeInBytes != size, "IndexBufferObject GL_BUFFER_SIZE failed");
    }
    return indexBufferObject;
}

unsigned int texture_create(const std::string& filePath, const bool isRGBA) {
    j_assert(filePath.empty() == false, "FilePath missing, cannot intit texture ");
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
    j_assert(textureId != 0, "Id missing, cannot bind texture ");
    glBindTexture(GL_TEXTURE_2D, textureId);
}

struct FpsCounter {
    unsigned long frames;
    unsigned int displayFps;
    float previousCurrentTime;
    float currentTime;
    float lastFpsCalcTime;
    float deltaTime;
    float overflowedFpsCalcTime;
};

struct ApplicationState {
    FpsCounter fpsCounter;
};

void fps_frames_increment(FpsCounter* fpsCounter, const unsigned int frames) {
    fpsCounter->frames += frames;
}
void fps_deltatime_calculate(FpsCounter* fpsCounter) {
    fpsCounter->currentTime = (float)glfwGetTime();
    fpsCounter->deltaTime = fpsCounter->currentTime - fpsCounter->lastFpsCalcTime;
    fpsCounter->lastFpsCalcTime = fpsCounter->currentTime;
}

void fps_scuffed_calculate(FpsCounter* fpsCounter) {
    const float second = 1.0f;
    const float secondsElapsedFromPrevious = (fpsCounter->currentTime + fpsCounter->overflowedFpsCalcTime) - fpsCounter->previousCurrentTime;
    if (second < secondsElapsedFromPrevious) {
        fpsCounter->displayFps = fpsCounter->frames;
        fpsCounter->frames = 0;
        fpsCounter->previousCurrentTime = fpsCounter->currentTime;
        fpsCounter->overflowedFpsCalcTime = secondsElapsedFromPrevious - second;
    }
}

// Becnhmark vars
static float textRenderTime = 0.0f;
static int currentSecond = 0;

static FreeTypeFont gDebugFTFont = {};
static ApplicationState appState = {};

int application_run() {
    int result;
    result = glfwInit();
    j_assert(result == GLFW_TRUE, "glfwInit() failed");
    GLFWwindow* window = window_create(false);
    result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    j_assert(result == 1, "Failed to initialize OpenGL context GLAD");
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &result);
    printf("Using OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    printf("Maximum nr of vertex attributes supported: %d\n", result);
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
            trans = glm::rotate(trans, glm::radians(rotateScale * -4), glm::vec3(0.0f, 0.0f, 1.0f));
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
            stream << std::fixed << std::setprecision(1) << appState.fpsCounter.currentTime;
            displayCurrent = "Time: " + stream.str() + "s";
            stream.str("");
            stream << std::fixed << std::setprecision(1) << appState.fpsCounter.displayFps;
            displayFps = "FPS: " + stream.str();
            if ((int)appState.fpsCounter.currentTime != currentSecond) {
                currentSecond = (int)appState.fpsCounter.currentTime;
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
        fps_frames_increment(&appState.fpsCounter, 1);
        fps_deltatime_calculate(&appState.fpsCounter);
        fps_scuffed_calculate(&appState.fpsCounter);
    }
    return 0;
}


int main() {
    int code = 0;
    std::setlocale(LC_ALL, "utf-8");
	code = application_run();
	return code;
}
