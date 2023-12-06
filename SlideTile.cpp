#include "SlideTile.h"
void SlideTile::load() {
	shape.init_buffer();
	init_texture("texture/goomba.png");
}
void SlideTile::handle_collision(Cube& cube) {
	while (!cube.try_slide(dir));
}
SlideTile::SlideTile() {
	update_world();
}
SlideTile::SlideTile(float x, float z) :Tile(x,z){

}