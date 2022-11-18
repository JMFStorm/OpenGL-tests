#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

unsigned int shader_create(const std::string& vertexFilePath, const std::string& fragmentFilePath);
unsigned int shader_get_uniform(int shaderId, const std::string& name);
void shader_use(unsigned int shaderId);
void shader_set_bool(int shaderId, const std::string& name, const bool value);
void shader_set_int(int shaderId, const std::string& name, const int value);
void shader_set_float(int shaderId, const std::string& name, const float value);
void shader_set_vec2(int shaderId, const std::string& name, const glm::vec2& value);
void shader_set_vec2(int shaderId, const std::string& name, const float x, const float y);
void shader_set_vec3(int shaderId, const std::string& name, const glm::vec3& value);
void shader_set_vec3(int shaderId, const std::string& name, float x, float y, float z);
void shader_set_vec4(int shaderId, const std::string& name, const glm::vec4& value);
void shader_set_vec4(int shaderId, const std::string& name, float x, float y, float z, float w);
void shader_set_mat2(int shaderId, const std::string& name, const glm::mat2& mat);
void shader_set_mat3(int shaderId, const std::string& name, const glm::mat3& mat);
void shader_set_mat4(int shaderId, const std::string& name, const glm::mat4& mat);

enum class ShaderDataType
{
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

GLenum shader_type_to_opengl_type(ShaderDataType type);
