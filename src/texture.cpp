#include <glad/glad.h>
#include "image.hpp"
#include "texture.hpp"
#include "j_util.hpp"

unsigned int texture_create(const std::string& file_path, const bool is_rgba) {
    j_assert(file_path.empty() == false, "FilePath missing, cannot init texture");
    unsigned int texture_id;
    int rgb_mode = is_rgba ? GL_RGBA : GL_RGB;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channels;
    unsigned char* data = image_load(file_path.c_str(), &width, &height, &channels);
    j_assert(data, "Failed to load texture: " + file_path);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgb_mode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    image_free_data(data);
    return texture_id;
}

void texture_bind(unsigned int textureId) {
    j_assert(textureId != 0, "Id missing, cannot bind texture");
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void texture_unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
