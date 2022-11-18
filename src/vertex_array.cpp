#include "vertex_array.hpp"

unsigned int vertex_array_create() {
    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    return vertexArrayObject;
}

void vertex_array_bind(unsigned int vertexArrayObject) {
    glBindVertexArray(vertexArrayObject);
}

void vertex_array_unbind() {
    glBindVertexArray(0);
}

void vertex_array_set_attribute_pointer(unsigned int attributeIndex, int componentsSize, ShaderDataType type, bool normalize, int strideByteSize, void* offset) {
    unsigned int glType = shader_type_to_opengl_type(type);
    GLboolean setNormalize = normalize ? GL_TRUE : GL_FALSE;
    glEnableVertexAttribArray(attributeIndex);
    glVertexAttribPointer(attributeIndex, componentsSize, glType, setNormalize, strideByteSize, offset);
}
