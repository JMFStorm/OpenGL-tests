#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* LoadImage(const char* filePath, int* width, int* height, int* nrChannels) {
    stbi_set_flip_vertically_on_load(true);
    return stbi_load(filePath, width, height, nrChannels, 0);
}

void FreeImageData(unsigned char* data) {
    stbi_image_free(data);
}
