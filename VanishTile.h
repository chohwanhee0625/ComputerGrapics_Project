#pragma once
#include "Tile.h"

class VanishTile :public Tile {
	bool isVanish{ false };
public:
	VanishTile() = delete;
	VanishTile(float x, float z);

	void load() override;
	void handle_collision(Cube& cube) override;

	bool get_isVanish() const;
};
