#pragma once
#include "Tile.h"
class GoalTile:public Tile {
	bool isGoal{ false };
public:
	GoalTile() = delete;
	GoalTile(float x, float z);
	void handle_collision(Cube& cube) override;
	bool get_isGoal() const;
	static void load();
};