#pragma once
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "Tile.h"
void timer(int key);
class GameManager {
	Light light{ {0,3,3} };
	Cube cube;
	vector<Tile> tiles{ {0,0},{0,1},{0,2} };
	Camera camera{ {0,3,3} };
public:
	GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	void load_scene();
	void render() const;
	void handle_key(unsigned char key);
	void handle_special_key(int key);
	void animation(int key);
	bool is_collide(const Cube& cube, const Tile& tile) const;
	void handle_collison();
};