#include "vertexArray.hpp"

unsigned int CreateVertexArray() {
    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    return vertexArrayObject;
}

void BindVertexArray(unsigned int vertexArrayObject) {
    glBindVertexArray(vertexArrayObject);
}

void UnbindVertexArray() {
    glBindVertexArray(0);
}

void SetVertexAttributePointer(unsigned int attributeIndex, int componentsSize, ShaderDataType type, bool normalize, int strideByteSize, void* offset) {
    unsigned int glType = ShaderDataTypeToOpenGLBaseType(type);
    GLboolean setNormalize = normalize ? GL_TRUE : GL_FALSE;
    glEnableVertexAttribArray(attributeIndex);
    glVertexAttribPointer(attributeIndex, componentsSize, glType, setNormalize, strideByteSize, offset);
}
