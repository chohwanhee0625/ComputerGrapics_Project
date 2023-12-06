#pragma once
#include "Tile.h"

class SlideTile: public Tile{
	string dir{ "BACK" };
	map<string, string> img_map{ {"FRONT","texture/FrontSlide.png"},{"BACK","texture/BackSlide.png"},{"LEFT","texture/LeftSlide.png"},{"RIGHT","texture/RightSlide.png"} };
public:
	SlideTile() = delete;
	SlideTile(const string& dir, float x, float z);
	void load() override;
	void handle_collision(Cube& cube) override;
};
