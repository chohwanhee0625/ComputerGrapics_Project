#include "SlideTile.h"
void SlideTile::handle_collision(Cube& cube) {
	while (!cube.try_slide(dir));
}

SlideTile::SlideTile(const string& dir, float x, float z) :Tile(x, z), dir{ dir }{img_name = img_map[dir]; }