#define STB_IMAGE_IMPLEMENTATION
#include "Cube.h"
#include "stb_image.h"
Shape Cube::shape{ "cube.obj" };

void Cube::init_buffer() {
	shape.init_buffer();
	init_texture();
}

void Cube::init_texture() {
	int widthImg, heightImg, numberOfChannel;
	stbi_set_flip_vertically_on_load(true);
	vector<string> fileNames{ "texture/mario.png","texture/luigi.png","texture/kinopio.png",
		"texture/koopa.png" ,"texture/goomba.png" ,"texture/peach.png" };
	for (int i = 0; i < fileNames.size(); ++i) {
		unsigned char* data = stbi_load(fileNames[i].c_str(), &widthImg, &heightImg, &numberOfChannel, 0);
		if (data != NULL) {
			glGenTextures(1, &texture[i]);

			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			if (numberOfChannel == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, numberOfChannel, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else if (numberOfChannel == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, numberOfChannel, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			stbi_image_free(data);
		}
		else {
			cout << "fail to load image" << endl;
		}
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

	auto viewPos = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3fv(viewPos, 1, glm::value_ptr(eye));

	auto color_loc = glGetUniformLocation(shaderProgramID, "color");
	glUniform4fv(color_loc, 1, glm::value_ptr(color));

	light.lighting();

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(int)));
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

void Cube::move(const string& dir) {
	R = glm::translate(glm::mat4(1), (this->*(move_rule[dir]))()) *
		glm::rotate(glm::mat4(1), glm::radians(5.f), rotate_axis[dir]) *
		glm::translate(glm::mat4(1), -(this->*(move_rule[dir]))()) * R;
	update_world();
}
void Cube::fall() {
	T *= glm::translate(glm::mat4(1), glm::vec3(0, -0.1, 0));
	update_world();
}
void Cube::slide(const glm::vec3& dir) {
	T *= glm::translate(glm::mat4(1), dir);
	update_world();
}

void Cube::change_axis(const string& dir) {
	coord_space = glm::rotate(glm::mat4(1), glm::radians(90.f), rotate_axis[dir]) * coord_space;
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

glm::vec3 Cube::get_center() const {
	return world * glm::vec4(0, 0.5, 0, 1);
}

glm::vec4 Cube::get_floor_center() const {
	return  world * faces[floor_id];
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

glm::vec3 Cube::get_floor_lt() const {
	auto center = get_floor_center();
	center.x -= 0.25;
	center.z -= 0.25;
	return center;
}

glm::vec3 Cube::get_floor_rb() const {
	auto center = get_floor_center();
	center.x += 0.25;
	center.z += 0.25;
	return center;
}