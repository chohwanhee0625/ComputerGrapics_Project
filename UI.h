#pragma once
#include "Shape.h"
#include "MySound.h"
class UI {
	static Shape shape;
	unsigned int texture;
	glm::mat4 world{ 1 };
	unique_ptr<MySound> sound;
public:
	static void Init_buffer();
	void load(const string& img_name, const char* sound_name, bool loop = false, float volume = 0.1f);
	void draw() const;
	void chage_img(const string& new_img_name);
	void resize(float sx, float sy, float sz);
	void move(float dx, float dy, float dz);
	bool isIn(float x, float y);
	void play_sound();
	void stop_sound();
};