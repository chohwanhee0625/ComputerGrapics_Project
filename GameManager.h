#pragma once
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "Tile.h"
#include "SlideTile.h"
#include "VanishTile.h"

void timer(int key);
class GameManager {
	Light light{ {0,3,3} };
	Cube cube;
	vector<unique_ptr<Tile>> tiles;
	Camera camera{ {0,3,3} };
	vector<string> stages{"stage0.txt"};
	int stage{ 0 };

public:
	GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	void load_stage();
	void render() const;
	void handle_key(unsigned char key);
	void handle_special_key(int key);
	void animation(int key);
	bool is_collide(const Cube& cube, const Tile& tile) const;
	void handle_collison();
};