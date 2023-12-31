#include "Tile.h"


Shape Tile::shape{ "cube.obj" };
static unsigned int texture;

Tile::Tile(float x, float z) {
	T = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
	texture = &::texture;
	update_world();
}

void Tile::load() {
	shape.init_buffer();
	::init_texture(::texture, "texture/tile.png");
}

void Tile::draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const {
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
	glBindTexture(GL_TEXTURE_2D, *texture);
	glDrawElements(GL_TRIANGLES, shape.indeices.size(), GL_UNSIGNED_INT, 0);
}

void Tile::update_world() {
	world = T * R * S;
	normal_world = glm::transpose(glm::inverse(world));
}

void Tile::handle_collision(Cube& cube){}

void Tile::goal_animation() {
	glm::vec3 dir_v = glm::normalize(world * glm::vec4(0, 0, 0, 1));

	T *= glm::translate(glm::mat4(1), glm::vec3(0.2 * dir_v.x, tick, 0.2 * dir_v.z));

	tick += 0.05;
	update_world();
}


glm::vec3 Tile::get_lb() const {
	auto lt = world * glm::vec4(-0.5, 0, -0.5, 1);
	return lt;
}  

glm::vec3 Tile::get_rt() const {
	auto rb = world * glm::vec4(0.5, 0, 0.5, 1);
	return rb;
}
