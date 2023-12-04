#pragma once
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
void timer(int key);
class GameManager {
	Light light{ {0,3,3} };
	Cube cube;
	Camera camera{ {0,3,3} };
	string move_dir{ "NONE" };
public:
	GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	void load_scene();
	void render() const;
	void handle_key(unsigned char key);
	void handle_special_key(int key);
	void animation(int key);
};