#include "shader.hpp"
#include <glad/glad.h>
#include "readFile.hpp"

unsigned int shader_create(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
    int success;
    char infoLog[512];
    // Get shader source strings
    std::string vertexSourceString = file_read_to_string(vertexFilePath);
    std::string fragmentSourceString = file_read_to_string(fragmentFilePath);
    const char* vertexSource = vertexSourceString.c_str();
    const char* fragmentSource = fragmentSourceString.c_str();
    // Create vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
    }
    // Create fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed: %s\n", infoLog);
    }
    // Create new shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Program linking failed: %s\n", infoLog);
    }
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void shader_use(unsigned int shaderId) {
    glUseProgram(shaderId);
}

unsigned int shader_get_uniform(int shaderId, const std::string& name) {
    return glGetUniformLocation(shaderId, name.c_str());
}

void shader_set_bool(int shaderId, const std::string& name, const bool value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)value);
}

void shader_set_int(int shaderId, const std::string& name, const int value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void shader_set_float(int shaderId, const std::string& name, const float value) {
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void shader_set_vec2(int shaderId, const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void shader_set_vec2(int shaderId, const std::string& name, const float x, const float y) {
    glUniform2f(glGetUniformLocation(shaderId, name.c_str()), x, y);
}

void shader_set_vec3(int shaderId, const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void shader_set_vec3(int shaderId, const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(shaderId, name.c_str()), x, y, z);
}

void shader_set_vec4(int shaderId, const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value[0]);
}

void shader_set_vec4(int shaderId, const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(shaderId, name.c_str()), x, y, z, w);
}

void shader_set_mat2(int shaderId, const std::string& name, const glm::mat2& mat) {
    glUniformMatrix2fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader_set_mat3(int shaderId, const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader_set_mat4(int shaderId, const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


GLenum shader_type_to_opengl_type(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Bool:     return GL_BOOL;
    }
    return 0;
}
