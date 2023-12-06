#include "VanishTile.h"

void VanishTile::load() {
	shape.init_buffer();
	init_texture("texture/peach.png");
}

void VanishTile::handle_collision(Cube& cube) {
	isCollide = true;
}

void VanishTile::set_isVanish(bool b) {
	isVanish = b;
}

void VanishTile::draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const {
	if (!isVanish) {
		Tile::draw(view, proj, eye, light);
	}
}

bool VanishTile::get_isCollide() const {
	return isCollide;
}

bool VanishTile::get_isVanish() const {
	return isVanish;
}

void VanishTile::reset() {
	isVanish = false;
	isCollide = false;
}
VanishTile::VanishTile(float x, float z) : Tile(x, z){}