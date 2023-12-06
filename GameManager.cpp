#include "GameManager.h"
GameManager GM;
extern ostream& operator<<(ostream& out, const glm::vec4& vec);
extern ostream& operator<<(ostream& out, const glm::vec3& vec);

bool GameManager::is_collide(const Cube& cube, const Tile& tile) const {
	auto cube_l = cube.get_lb().x, cube_b = cube.get_lb().z, cube_r = cube.get_rt().x, cube_t = cube.get_rt().z;
	auto tile_l = tile.get_lb().x, tile_b = tile.get_lb().z, tile_r = tile.get_rt().x, tile_t = tile.get_rt().z;

	if (cube_l > tile_r) {
		return false;
	}
	if (cube_r < tile_l) {
		return false;
	}
	if (cube_b > tile_t) {
		return false;
	}
	if (cube_t < tile_b) {
		return false;
	}

	return true;
}

void GameManager::handle_collison() {
	bool isFall = true;
	for (auto& tile : tiles) {
		if (is_collide(cube, *tile.get())) {
			tile->handle_collision(cube);
			isFall = false;
		}
	}

	if (isFall) {
		while (!cube.try_fall());
	}
}

void timer(int key) {
	GM.animation(key);
}

void GameManager::load_stage() {
	glutTimerFunc(10, timer, 0);
	cube.init_buffer();
	tiles.emplace_back(new Tile(0, 0));
	tiles.emplace_back(new SlideTile("BACK", 0, 1));
	tiles.emplace_back(new Tile(0, 2));
	tiles.emplace_back(new Tile(0, 3));
	tiles.emplace_back(new Tile(0, 4));
	for (auto& tile : tiles) {
		tile->load();
	}
}

void GameManager::render() const {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	cube.draw(camera.view, camera.proj, camera.EYE, light);
	for (const auto& tile : tiles) {
		tile->draw(camera.view, camera.proj, camera.EYE, light);
	}
	glutSwapBuffers();
}

void GameManager::handle_key(unsigned char key) {
	cube.handle_key(key);
	if (key == 'f') {
		cube.fall();
	}
	else if (key == 'z') {
		cube.try_slide("FRONT");
	}
	else if (key == 'x') {
		cube.try_slide("BACK");
	}
	else if (key == 'c') {
		cube.try_slide("LEFT");
	}
	else if (key == 'v') {
		cube.try_slide("RIGHT"); 
	}
	else if (key == 'q') {
		cout << cube.get_center() << endl;
	}
	glutPostRedisplay();
}

void GameManager::handle_special_key(int key) {
	glutPostRedisplay();
}

void GameManager::animation(int key) {
	if (cube.update()) {
		handle_collison();
	}
	glutTimerFunc(10, timer, 0);
	glutPostRedisplay();
}
