#pragma once
#include "Shape.h"
struct Rect {
	static Shape shape;
	glm::mat4 world, normal_world;
	glm::mat4 Rx{ 1.f }, Ry{ 1 }, Rz{ 1.f }, S{ 1.f }, T{ 1.f };
	glm::vec4 O{ 0,0,0,1 }, pivot{ 0,0,0,1 };
	glm::vec4 color{ randomColor(dre), randomColor(dre), randomColor(dre), 1 };
	glm::vec3 size{ 1,1,1 };
	glm::vec3 pos;
	glm::vec3 theta{ 0,0,0 };
	float sh = 0;
	Rect() {
		update_world();
	}
	void init_buffer() {
		shape.init_buffer();
	}
	void draw() const {
		glBindVertexArray(shape.VAO);
		auto world_location = glGetUniformLocation(shaderProgramID, "world");
		glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(world));
		auto tiworld_location = glGetUniformLocation(shaderProgramID, "tranInvWorld");
		glUniformMatrix4fv(tiworld_location, 1, GL_FALSE, glm::value_ptr(normal_world));
		auto color_loc = glGetUniformLocation(shaderProgramID, "color");
		glUniform4f(color_loc, 1, 1, 1, 1);
		glDrawElements(GL_TRIANGLES, shape.indeices.size(), GL_UNSIGNED_INT, 0);
	}
	void update_world() {
		world = T * Rx * Ry * Rz * S;
		normal_world = glm::transpose(glm::inverse(world));
	}
	void rotate_y() {
		theta.y += 5;
		Ry = glm::rotate(glm::mat4(1), glm::radians(theta.y), glm::vec3(0, 1, 0));
		update_world();
	}
	void move(const glm::vec3& dir) {
		pos += dir;
		T = glm::translate(glm::mat4(1), pos);
		update_world();
	}
	void move(float dx, float dy, float dz) {
		pos += glm::vec3(dx, dy, dz);
		T *= glm::translate(glm::mat4(1), glm::vec3(dx, dy, dz));
		update_world();
	}
	void resize(float sx, float sy, float sz) {
		size = glm::vec3(sx, sy, sz);
		S = glm::scale(glm::mat4(1), size);
		update_world();
	}
	void resize(float s) {
		size = glm::vec3(s, s, s);
		S = glm::scale(glm::mat4(1), size);
		update_world();
	}
	void rotate_y_at_O() {
		T = glm::rotate(glm::mat4(1), glm::radians(5.f), glm::vec3(0, 1, 0)) * T;
		update_world();
	}
	glm::vec3 get_center() const {
		return world * glm::vec4(0, 0, 0, 1);
	}
};