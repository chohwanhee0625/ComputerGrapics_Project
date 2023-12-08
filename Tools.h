#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;

void window2GL(int x, int y, int w, int h, float& glX, float& glY);

char* filetobuf(const char* file);

void init_texture(unsigned int& texture, const string& file_name, bool set_flip = true);

unsigned char* my_load_image(const string& file_name, int* width, int* height, int* channels, bool set_flip = true);

void my_image_free(unsigned char* data);