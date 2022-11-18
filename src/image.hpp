#pragma once

unsigned char* image_load(const char* filePath, int* width, int* height, int* nrChannels);
void image_free_data(unsigned char* data);
