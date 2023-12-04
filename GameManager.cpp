#include "GameManager.h"
GameManager GM;

void timer(int key) {
	GM.animation(key);
}

void GameManager::load_scene() {
	cube.init_buffer();
	cube.update_world();
}

void GameManager::render() const {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	cube.draw(camera.view, camera.proj, camera.EYE, light);
	glutSwapBuffers();
}

void GameManager::handle_key(unsigned char key) {
	if (key == 'w' && move_dir == "NONE") {
		move_dir = "FRONT";
		glutTimerFunc(10, timer, 0);
	}
	else if (key == 's' && move_dir == "NONE") {
		move_dir = "BACK";
		glutTimerFunc(10, timer, 0);
	}
	else if (key == 'a' && move_dir == "NONE") {
		move_dir = "LEFT";
		glutTimerFunc(10, timer, 0);
	}
	else if (key == 'd' && move_dir == "NONE") {
		move_dir = "RIGHT";
		glutTimerFunc(10, timer, 0);
	}
	else if (key == 'c') {
		cube.fall();
	}
	else if (key == 'r') {
		cube.slide({ 0,0,1 });
		cube.check_floor_face();
	}
	else if (key == 'q') {
		exit(0);
	}
	glutPostRedisplay();
}

void GameManager::handle_special_key(int key) {
	glutPostRedisplay();
}

void GameManager::animation(int key) {
	if (move_dir != "NONE") {
		static int i = 0;
		if (i >= 90 / 5) {
			i = 0;
			cube.change_axis(move_dir);
			move_dir = "NONE";
			cube.check_floor_face();
		}
		else {
			cube.move(move_dir);
			++i;
			glutTimerFunc(10, timer, 0);
			glutPostRedisplay();
		}
	}
}
