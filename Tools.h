#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

void window2GL(int x, int y, int w, int h, float& glX, float& glY);

char* filetobuf(const char* file);

unsigned char* my_load_image(const string& file_name, int* width, int* height, int* channels, bool set_flip = true);

void my_image_free(unsigned char* data);