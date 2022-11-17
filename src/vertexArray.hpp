#pragma once
#include <glad/glad.h>
#include "shader.hpp"

unsigned int CreateVertexArray();
void BindVertexArray(unsigned int vertexArrayObject);
void UnbindVertexArray();
void SetVertexAttributePointer(unsigned int attributeIndex, int componentsSize, ShaderDataType type, bool normalize, int strideByteSize, void* offset);
