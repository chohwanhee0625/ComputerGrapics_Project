#include "GameManager.h"
#include <filesystem>
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
		auto p = dynamic_cast<VanishTile*>(tile.get());
		if (p != nullptr) {
			if (p->get_isCollide()) {
				p->set_isVanish(true);
				continue;
			}
		}

	}
	for (auto& tile : tiles) {
		auto p = dynamic_cast<VanishTile*>(tile.get());
		if (p != nullptr) {
			if (p->get_isVanish()) {
				continue;
			}
		}
		if (is_collide(cube, *tile.get())) {
			tile->handle_collision(cube);
			isFall = false;
		}
	}

	auto p = dynamic_cast<GoalTile*>(tiles.back().get());
	if (p->get_isGoal()) {
		glutTimerFunc(10, timer, 1);
	}

	if (isFall) {
		while (!cube.try_fall());
	}
}

void timer(int key) {
	GM.animation(key);
}

void GameManager::restart() {
	cube.reset();
	for (auto& tile : tiles) {
		auto p = dynamic_cast<VanishTile*>(tile.get());
		if (p != nullptr) {
			p->reset();
		}
	}
}
GameManager::GameManager() {
	for (const auto& iter : filesystem::directory_iterator("stages")) {
		stages.emplace_back(iter.path().string());
	}
}
void GameManager::load_stage() {
	cube.init_buffer();
	ifstream file(stages[stage]);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << stages[stage] << std::endl;
		return;
	}

	string line;
	while (std::getline(file, line)) {
		istringstream iss(line);
		string token;
		iss >> token;

		if (token == "T") {
			// 일반 타일
			float x, z;
			iss >> x >> z;
			tiles.emplace_back(new Tile(x, z));
			tiles.back()->load();
		}
		else if (token == "S") {
			// 슬라이드 타일
			string dir;
			float x, z;
			iss >> dir >> x >> z;
			tiles.emplace_back(new SlideTile(dir, x, z));
			tiles.back()->load();
		}
		else if (token == "V") {
			// 사라지는 타일
			float x, z;
			iss >> x >> z;
			tiles.emplace_back(new VanishTile(x, z));
			tiles.back()->load();
		}
		else if (token == "G") {
			float x, z;
			iss >> x >> z;
			tiles.emplace_back(new GoalTile(x, z));
			tiles.back()->load();
		}
	}
	file.close();
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
	if (key == 'r' || key == 'R') {
		restart();
	}
	glutPostRedisplay();
}

void GameManager::handle_special_key(int key) {
	glutPostRedisplay();
}
void GameManager::next_stage() {
	cube.reset();
	tiles.clear();

	stage = min(stage + 1, int(stages.size() - 1));
	load_stage();
}
void GameManager::animation(int key) {
	if (key == 0) {
		if (cube.update()) {
			handle_collison();
		}
		glutTimerFunc(10, timer, 0);
		if (cube.get_center().y < -10) {
			restart();
		}
	}
	else if (key == 1) {
		static int i = 0;
		cout << "victory" << endl;
		++i;
		if (i < 20) {
			glutTimerFunc(10, timer, 1);
		}
		else {
			i = 0;
			next_stage();
		}
	}
	glutPostRedisplay();
}
