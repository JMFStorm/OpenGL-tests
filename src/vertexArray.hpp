#pragma once
#include <glad/glad.h>
#include "shader.hpp"

unsigned int vertex_array_create();
void vertex_array_bind(unsigned int vertexArrayObject);
void vertex_array_unbind();
void vertex_array_set_attribute_pointer(unsigned int attributeIndex, int componentsSize, ShaderDataType type, bool normalize, int strideByteSize, void* offset);
