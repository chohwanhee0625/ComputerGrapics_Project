#pragma once
#include "Shape.h"
class UI {
	static Shape shape;
	unsigned int texture;
	glm::mat4 world{ 1 };
public:
	void load(const string& img_name);
	void draw() const;
	void chage_img(const string& new_img_name);
	void resize(float sx, float sy, float sz);
	void move(float dx, float dy, float dz);
	bool isIn(float x, float y);
};