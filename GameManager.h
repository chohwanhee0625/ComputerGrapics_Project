#pragma once
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "Tile.h"
#include "SlideTile.h"
#include "VanishTile.h"
#include "GoalTile.h"
#include "UI.h"

#define TITLE_MODE 0
#define PLAY_MODE 1
#define END_MODE 2
void timer(int key);
class GameManager {
	Light light{ {-3,3,3} };
	Cube cube;
	vector<unique_ptr<Tile>> tiles;
	Camera camera{ {1,6,5} };
	vector<string> stages;
	UI bg, play_button, ending;
	int stage{ 8 };
	int mode{ TITLE_MODE };

public:
	GameManager() = default;
	~GameManager();
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
	void load_game();
	void handle_mouse(int key, int state, float x, float y);
};