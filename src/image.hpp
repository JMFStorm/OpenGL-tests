#pragma once

unsigned char* LoadImage(const char* filePath, int* width, int* height, int* nrChannels);
void FreeImageData(unsigned char* data);