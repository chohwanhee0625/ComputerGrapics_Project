#pragma once
#include "Tile.h"

class SlideTile: public Tile{
	string dir{ "BACK" };
	map<string, string> img_map{ {"FRONT","texture/goomba.png"},{"BACK","texture/koopa.png"},{"LEFT","texture/luigi.png"},{"RIGHT","texture/kinopio.png"} };
public:
	SlideTile() = delete;
	SlideTile(const string& dir, float x, float z);
	void load() override;
	void handle_collision(Cube& cube) override;
};
