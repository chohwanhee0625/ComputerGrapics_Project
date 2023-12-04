#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;
void window2GL(int x, int y, int w, int h, float& glX, float& glY);

char* filetobuf(const char* file);