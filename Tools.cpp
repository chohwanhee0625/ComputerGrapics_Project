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

void init_texture(unsigned int& texture, const string& file_name, bool set_flip) {
	int widthImg, heightImg, numberOfChannel;
	stbi_set_flip_vertically_on_load(set_flip);
	unsigned char* data = stbi_load(file_name.c_str(), &widthImg, &heightImg, &numberOfChannel, 0);

	if (data != NULL) {
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (numberOfChannel == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, numberOfChannel, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (numberOfChannel == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, numberOfChannel, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
	}
	else {
		cout << "fail to load image" << endl;
	}
}

unsigned char* my_load_image(const string& file_name, int* width, int* height, int* channels, bool set_flip) {
	stbi_set_flip_vertically_on_load(set_flip);
	return stbi_load(file_name.c_str(), width, height, channels, 0);
}

void my_image_free(unsigned char* data) {
	stbi_image_free(data);
}
