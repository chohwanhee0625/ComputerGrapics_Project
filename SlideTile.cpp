#include "SlideTile.h"
map<string,string> img_map{ 
	{"FRONT", "texture/FrontSlide.png"}, 
	{ "BACK","texture/BackSlide.png" }, 
	{ "LEFT","texture/LeftSlide.png" }, 
	{ "RIGHT","texture/RightSlide.png" } };
map<string, unsigned int> texture_map;
void SlideTile::handle_collision(Cube& cube) {
	cube.try_slide(dir);
}
void SlideTile::load() {
	for (auto iter : ::img_map) {
		init_texture(texture_map[iter.first], iter.second);
	}
}
SlideTile::SlideTile(const string& dir, float x, float z) :Tile(x, z), dir{ dir }{ texture = &texture_map[dir]; }