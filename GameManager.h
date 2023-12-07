#pragma once
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "Tile.h"
#include "SlideTile.h"
#include "VanishTile.h"
#include "GoalTile.h"

void timer(int key);
class GameManager {
	Light light{ {0,3,3} };
	Cube cube;
	vector<unique_ptr<Tile>> tiles;
	Camera camera{ {1,6,5} };
	vector<string> stages;
	int stage{ 0 };

public:
	GameManager();
	GameManager& operator=(const GameManager&) = delete;
	void next_stage();
	void load_stage();
	void render() const;
	void handle_key(unsigned char key);
	void handle_special_key(int key);
	void animation(int key);
	bool is_collide(const Cube& cube, const Tile& tile) const;
	void handle_collison();
	void restart();
};