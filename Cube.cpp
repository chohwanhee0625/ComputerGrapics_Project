#include "Cube.h"
#include "Tools.h"

Shape Cube::shape{ "cube.obj" };
unsigned int Cube::texture[6];
unique_ptr<MySound> Cube::move_sound;
unique_ptr<MySound> Cube::fall_sound;
unique_ptr<MySound> Cube::slide_sound;


void Cube::load() {
	shape.init_buffer();
	move_sound.reset(new MySound("sound/cube_move.wav", false));
	fall_sound.reset(new MySound("sound/cube_fall.wav", false));
	slide_sound.reset(new MySound("sound/cube_slide.wav", false));

	vector<string> file_names{ "texture/mario.png","texture/luigi.png","texture/kinopio.png",
	"texture/koopa.png" ,"texture/goomba.png" ,"texture/peach.png" };
	glGenTextures(6, Cube::texture);
	for (int i = 0; i < file_names.size(); ++i) {
		init_texture(Cube::texture[i], file_names[i].c_str());
	}
}

void Cube::draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const {
	glBindVertexArray(shape.VAO);

	auto view_location = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	auto proj_location = glGetUniformLocation(shaderProgramID, "proj");
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));

	auto world_location = glGetUniformLocation(shaderProgramID, "world");
	glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(world));

	auto tiworld_location = glGetUniformLocation(shaderProgramID, "normalWorld");
	glUniformMatrix4fv(tiworld_location, 1, GL_FALSE, glm::value_ptr(normal_world));

	auto color_loc = glGetUniformLocation(shaderProgramID, "color");
	glUniform4fv(color_loc, 1, glm::value_ptr(color));

	light.lighting();

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, Cube::texture[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(int)));
	}
}

bool Cube::update() {
	if (state == "MOVE") {
		return move();
	}
	else if (state == "SLIDE") {
		return slide();
	}
	else if (state == "FALL") {
		fall();
	}
	return false;
}

void Cube::reset() {
	set_Idle();

	S = glm::scale(glm::mat4(1),glm::vec3(1,2,1));
	T = glm::mat4(1);
	R = glm::mat4(1);

	update_world();

	coord_space = glm::mat4(1);
	floor_id = BOTTOM_FACE;
	degree = 0;
}

void Cube::handle_key(unsigned char key) {
	if (state == "IDLE") {
		if (key == 'w') {
			move_sound->play();
			state = "MOVE";
			dir = "FRONT";
		}
		else if (key == 's') {
			move_sound->play();
			state = "MOVE";
			dir = "BACK";
		}
		else if (key == 'a') {
			move_sound->play();
			state = "MOVE";
			dir = "LEFT";
		}
		else if (key == 'd') {
			move_sound->play();
			state = "MOVE";
			dir = "RIGHT";
		}
	}
}

void Cube::update_world() {
	world = T * R * S;
	normal_world = glm::transpose(glm::inverse(world));
}

void Cube::resize(float sx, float sy, float sz) {
	S = glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz));
	update_world();
}

bool Cube::move() {
	R = glm::translate(glm::mat4(1), (this->*(move_rule[dir]))()) *
		glm::rotate(glm::mat4(1), glm::radians(5.f), rotate_axis[dir]) *
		glm::translate(glm::mat4(1), -(this->*(move_rule[dir]))()) * R;
	degree += 5;
	if (degree == 90) {
		degree = 0;
		change_space();
		check_floor_face();
		set_Idle();
		update_world();
		return true;
	}
	update_world();
	return false;
}
bool Cube::slide() {
	T *= glm::translate(glm::mat4(1), 0.1f * slide_dir[dir]);
	degree += 10;
	if (degree == 100) {
		degree = 0;
		set_Idle();
		return true;
	}
	update_world();
	return false;
}
void Cube::fall() {
	T *= glm::translate(glm::mat4(1), glm::vec3(0, -0.1, 0));
	update_world();
}

void Cube::change_space() {
	coord_space = glm::rotate(glm::mat4(1), glm::radians(90.f), rotate_axis[dir]) * coord_space;
}
bool Cube::try_slide(const string& dir) {
	if (this->state == "IDLE") {
		this->state = "SLIDE";
		this->dir = dir;
		slide_sound->play();
		return true;
	}
	else {
		return false;
	}
}
bool Cube::try_fall() {
	if (this->state == "IDLE") {
		this->state = "FALL";
		this->dir = "NONE";
		fall_sound->play();
		return true;
	}
	else {
		return false;
	}
}
void Cube::check_floor_face() {
	float minY = 200;
	for (int i = 0; i < 6; ++i) {
		float y = (world * faces[i]).y;
		if (minY > y) {
			minY = y;
			floor_id = i;
		}
	}
}
void Cube::set_Idle() {
	state = "IDLE";
	dir = "NONE";
	move_sound->stop();
}


glm::vec3 Cube::get_front_edge() const {
	auto edge = faces[floor_id];
	float f = 0;
	int axis;
	for (int i = 0; i < 3; ++i) {
		if (f < abs(coord_space[i].z)) {
			axis = i;
			f = abs(coord_space[i].z);
		}
	}
	if (coord_space[axis].z < 0) {
		edge[axis] += 0.5;
	}
	else {
		edge[axis] -= 0.5;
	}
	return R * S * edge;
}

glm::vec3 Cube::get_back_edge() const {
	auto edge = faces[floor_id];
	float f = 0;
	int axis;
	for (int i = 0; i < 3; ++i) {
		if (f < abs(coord_space[i].z)) {
			axis = i;
			f = abs(coord_space[i].z);
		}
	}
	if (coord_space[axis].z > 0) {
		edge[axis] += 0.5;
	}
	else {
		edge[axis] -= 0.5;
	}
	return R * S * edge;
}

glm::vec3 Cube::get_left_edge() const {
	auto edge = faces[floor_id];
	float f = 0;
	int axis;
	for (int i = 0; i < 3; ++i) {
		if (f < abs(coord_space[i].x)) {
			axis = i;
			f = abs(coord_space[i].x);
		}
	}
	if (coord_space[axis].x < 0) {
		edge[axis] += 0.5;
	}
	else {
		edge[axis] -= 0.5;
	}
	return R * S * edge;
}

glm::vec3 Cube::get_right_edge() const {
	auto edge = faces[floor_id];
	float f = 0;
	int axis;
	for (int i = 0; i < 3; ++i) {
		if (f < abs(coord_space[i].x)) {
			axis = i;
			f = abs(coord_space[i].x);
		}
	}
	if (coord_space[axis].x > 0) {
		edge[axis] += 0.5;
	}
	else {
		edge[axis] -= 0.5;
	}
	return R * S * edge;
}

glm::vec3 Cube::get_center() const {
	return world * faces[floor_id];
}

glm::vec3 Cube::get_lb() const {
	auto center = get_center();
	center.x -= 0.25;
	center.z -= 0.25;
	return center;
}

glm::vec3 Cube::get_rt() const {
	auto center = get_center();
	center.x += 0.25;
	center.z += 0.25;
	return center;
}