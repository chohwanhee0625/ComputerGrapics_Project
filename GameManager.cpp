#include "GameManager.h"
GameManager GM;

void timer(int key) {
	GM.animation(key);
}

void GameManager::load_scene() {
	glutTimerFunc(10, timer, 0);
	cube.init_buffer();
}

void GameManager::render() const {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	cube.draw(camera.view, camera.proj, camera.EYE, light);
	glutSwapBuffers();
}

void GameManager::handle_key(unsigned char key) {
	cube.handle_key(key);
	if (key == 'f') {
		cube.fall();
	}
	else if (key == 'z') {
		cube.set_slide("FRONT");
	}
	else if (key == 'x') {
		cube.set_slide("BACK");
	}
	else if (key == 'c') {
		cube.set_slide("LEFT");
	}
	else if (key == 'v') {
		cube.set_slide("RIGHT");
	}
	else if (key == 'q') {
		cout << cube.get_floor_center() << endl;
	}
	glutPostRedisplay();
}

void GameManager::handle_special_key(int key) {
	glutPostRedisplay();
}

void GameManager::animation(int key) {
	cube.update();
	glutTimerFunc(10, timer, 0);
	glutPostRedisplay();
}
