#include "Tools.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

void window2GL(int x, int y, int w, int h, float& glX, float& glY) {
	glX = (x - w / 2.0) / (w / 2.0);
	glY = (h / 2.0 - y) / (h / 2.0);
}
char* filetobuf(const char* file) {
	ifstream in(file);
	if (in.fail()) {
		return nullptr;
	}

	in.seekg(0, ios::end);
	auto file_size = static_cast<size_t>(in.tellg());
	in.seekg(0, ios::beg);
	char* returnVal = new char[file_size + 1]{};
	in.read(returnVal, file_size);
	in.close();

	return returnVal;
}
unsigned char* my_load_image(const string& fileName, int* width, int* height, int* channels) {
	stbi_set_flip_vertically_on_load(true);
	return stbi_load(fileName.c_str(), width, height, channels, 0);
}

void my_image_free(unsigned char* data) {
	stbi_image_free(data);
}
