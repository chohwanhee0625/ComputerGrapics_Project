#include "SlideTile.h"
void SlideTile::load() {
	shape.init_buffer();
	init_texture(img_map[dir]);
}
void SlideTile::handle_collision(Cube& cube) {
	while (!cube.try_slide(dir));
}

SlideTile::SlideTile(const string& dir, float x, float z) : Tile(x, z), dir{ dir }{}