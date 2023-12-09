#include "VanishTile.h"

void VanishTile::handle_collision(Cube& cube) {
	auto cl = cube.get_lb().x, cb = cube.get_lb().z, cr = cube.get_rt().x, ct = cube.get_rt().z;
	auto l = get_lb().x, b = get_lb().z, r = get_rt().x, t = get_rt().z;

	if (l <= cl && cl <= r && l <= cr && cr <= r && b <= cb && cb <= t && b <= ct && ct <= t) {
		cube.try_fall();
		isVanish = true;
	}
	else {

	}
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
VanishTile::VanishTile(float x, float z) :Tile(x, z){
	color = glm::vec4(1, 0.5, 0.5, 1);
}