#pragma once
#include "Tile.h"

class VanishTile :public Tile {
	bool isVanish{ false };
	bool isCollide{ false };
public:
	VanishTile() = delete;
	VanishTile(float x, float z);

	void handle_collision(Cube& cube) override;
	void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const override;

	void reset();

	bool get_isCollide() const;
	bool get_isVanish() const;

	void set_isVanish(bool b);
};
