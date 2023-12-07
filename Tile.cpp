#define STB_IMAGE_IMPLEMENTATION
#include "Tile.h"
#include "Tools.h"


Shape Tile::shape{ "cube.obj" };

Tile::Tile(float x, float z) {
	T = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
	update_world();
}

void Tile::load() {
	shape.init_buffer();
	init_texture();
}

void Tile::init_texture() {
	int widthImg, heightImg, numberOfChannel;
	unsigned char* data = my_load_image(img_name.c_str(), &widthImg, &heightImg, &numberOfChannel);

	if (data != NULL) {
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
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
		my_image_free(data);
	}
	else {
		cout << "fail to load image" << endl;
	}
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
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, shape.indeices.size(), GL_UNSIGNED_INT, 0);
}

void Tile::update_world() {
	world = T * R * S;
	normal_world = glm::transpose(glm::inverse(world));
}

void Tile::handle_collision(Cube& cube){}

glm::vec3 Tile::get_lb() const {
	auto lt = world * glm::vec4(-0.5, 0, -0.5, 1);
	return lt;
}  

glm::vec3 Tile::get_rt() const {
	auto rb = world * glm::vec4(0.5, 0, 0.5, 1);
	return rb;
}
