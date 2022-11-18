#include "vertex_array.hpp"

unsigned int vertex_array_create() {
    unsigned int vertex_array;
    glGenVertexArrays(1, &vertex_array);
    return vertex_array;
}

void vertex_array_bind(unsigned int vertex_array) {
    glBindVertexArray(vertex_array);
}

void vertex_array_unbind() {
    glBindVertexArray(0);
}

void vertex_array_set_attribute_pointer(unsigned int attribute_index, int components_size, ShaderDataType type, bool normalize, int stride_byte_size, void* offset) {
    unsigned int gl_type = shader_type_to_opengl_type(type);
    GLboolean set_normalize = normalize ? GL_TRUE : GL_FALSE;
    glEnableVertexAttribArray(attribute_index);
    glVertexAttribPointer(attribute_index, components_size, gl_type, set_normalize, stride_byte_size, offset);
}
