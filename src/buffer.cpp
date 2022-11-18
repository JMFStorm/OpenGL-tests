#include <glad/glad.h>
#include "buffer.hpp"
#include "j_util.hpp"

unsigned int buffer_vertex_create(const std::vector<float> vertices) {
    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    int sizeInBytes = (int)vertices.size() * sizeof(unsigned int);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, &vertices[0], GL_STATIC_DRAW);
    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    j_assert(sizeInBytes == size, "VertexBufferObject GL_BUFFER_SIZE failed");
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
    j_assert(sizeInBytes == size, "IndexBufferObject GL_BUFFER_SIZE failed");
    return indexBufferObject;
}