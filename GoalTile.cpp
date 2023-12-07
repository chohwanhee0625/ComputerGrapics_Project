#include "GoalTile.h"
GoalTile::GoalTile(float x, float z) :Tile(x, z) {
	img_name = "texture/GoalTile.png";
}

void GoalTile::handle_collision(Cube& cube) {
	auto cl = cube.get_lb().x, cb = cube.get_lb().z, cr = cube.get_rt().x, ct = cube.get_rt().z;
	auto l = get_lb().x, b = get_lb().z, r = get_rt().x, t = get_rt().z;

	if (l <= cl && cl <= r && l <= cr && cr <= r && b <= cb && cb <= t && b <= ct && ct <= t) {
		isGoal = true;
	}
}

bool GoalTile::get_isGoal() const {
	return isGoal;
}