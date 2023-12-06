#pragma once
#include "Tile.h"

class SlideTile: public Tile{
	string dir{ "BACK" };
public:
	SlideTile();
	SlideTile(float x, float z);
	void load() override;
	void handle_collision(Cube& cube) override;
};
