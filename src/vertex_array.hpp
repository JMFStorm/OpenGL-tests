#pragma once
#include <glad/glad.h>
#include "shader.hpp"

unsigned int vertex_array_create();
void vertex_array_bind(unsigned int vertex_array);
void vertex_array_unbind();
void vertex_array_set_attribute_pointer(unsigned int attribute_index, int components_size, ShaderDataType type, bool normalize, int stride_byte_size, void* offset);
