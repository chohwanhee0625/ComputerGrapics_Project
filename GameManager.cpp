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
		stage_clear->play(1.8f);
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


void GameManager::load_game() {
	for (const auto& iter : filesystem::directory_iterator("stages")) {
		stages.emplace_back(iter.path().string());
	}
	MySound::Init();
	Cube::load();
	Tile::load();
	SlideTile::load();
	GoalTile::load();
	UI::Init_buffer();
	stage_clear.reset(new MySound("sound/clear.wav", false, 1.f));
	bg.load("texture/background.png", "sound/bgm.wav", true);
	bg.play_sound();
	play_button.load("texture/play_button.png","sound/click_button.wav");
	ending.load("texture/congratulation.png", "sound/victory.mp3", false, 1.f);
	play_button.resize(0.5, 0.1, 1);
	play_button.move(0, -0.25, -0.001);
}

void GameManager::load_stage() {
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
		}
		else if (token == "S") {
			// 슬라이드 타일
			string dir;
			float x, z;
			iss >> dir >> x >> z;
			tiles.emplace_back(new SlideTile(dir, x, z));
		}
		else if (token == "V") {
			// 사라지는 타일
			float x, z;
			iss >> x >> z;
			tiles.emplace_back(new VanishTile(x, z));
		}
		else if (token == "G") {
			//골 타일 무조건 마지막 줄에 있어야 함
			float x, z;
			iss >> x >> z;
			tiles.emplace_back(new GoalTile(x, z));
		}
		else if (token == "C") {
			glm::vec3 e, a, u;
			iss >> e.x >> e.y >> e.z >> a.x >> a.y >> a.z >> u.x >> u.y >> u.z;
			camera.set_view(e, a, u);
			light.pos = e;
		}
		else if (token == "CI") {
			float l, r, t, b;
			iss >> l >> r >> b >> t;
			camera.set_ortho(l, r, b, t);
		}
	}
	file.close();
}

void GameManager::render() const {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (mode != END_MODE) {
		cube.draw(camera.view, camera.proj, camera.EYE, light);
		bg.draw();
	}
	if (mode == PLAY_MODE) {
		for (const auto& tile : tiles) {
			tile->draw(camera.view, camera.proj, camera.EYE, light);
		}
	}
	if (mode == TITLE_MODE) {
		play_button.draw();
	}
	if (mode == END_MODE) {
		ending.draw();
	}
	glutSwapBuffers();
}

void GameManager::handle_key(unsigned char key) {
	if (mode != END_MODE) {
		cube.handle_key(key);
	}
	if (mode == PLAY_MODE && key == 'r' || key == 'R') {
		restart();
	}
	else if (key == 13 && mode == END_MODE) {
		mode = PLAY_MODE;
		ending.stop_sound();
		bg.play_sound();
		load_stage();
	}
	if (key == 27) {
		exit(1);
	}
	glutPostRedisplay();
}

void GameManager::handle_special_key(int key) {
	glutPostRedisplay();
}

void GameManager::next_stage() {
	cube.reset();
	tiles.clear();
	stage_clear->stop();
	++stage;
	if (stage < stages.size()) {
		load_stage();
	}
	else {
		mode = END_MODE;
		bg.stop_sound();
		ending.play_sound();
		stage = 0;
	}
}

void GameManager::animation(int key) {
	if (key == 0) {
		if (cube.update() && mode == PLAY_MODE) {
			handle_collison();
		}
		glutTimerFunc(10, timer, 0);
		if (cube.get_center().y < -10) {
			restart();
		}
	}
	else if (key == 1) {
		static int i = 0;

		for (const auto& tile : tiles) {
			tile->goal_animation();
		}
		++i;

		if (i < 40) {
			glutTimerFunc(20, timer, 1);
		}
		else {
			i = 0;
			next_stage();
		}
	}
	glutPostRedisplay();
}

void GameManager::handle_mouse(int key, int state, float x, float y) {
	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN && play_button.isIn(x,y) && mode == TITLE_MODE) {
		mode = PLAY_MODE;
		play_button.play_sound();
		cube.reset();
		load_stage();
	}
}
GameManager::~GameManager() {
	MySound::Release();
}