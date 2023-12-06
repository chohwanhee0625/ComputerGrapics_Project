#include "VanishTile.h"

void VanishTile::load() {
	shape.init_buffer();
	init_texture("texture/peach.png");
}
void VanishTile::handle_collision(Cube& cube) {
	isVanish = true;
}

bool VanishTile::get_isVanish() const {
	return isVanish;
}
VanishTile::VanishTile(float x, float z) : Tile(x, z){}