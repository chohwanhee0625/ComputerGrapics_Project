#include "UI.h"
#include "Tools.h"
static vector<Vector3> vertices{ {-1,1,0.99},{-1,-1,0.99},{1,-1,0.99},{1,1,0.99} };
static vector<Vector3> normals{ {0,0,-1},{0,0,-1} ,{0,0,-1} ,{0,0,-1} };
static vector<Vector2> textures{ {0,0},{0,1},{1,1},{1,0} };
static vector<int> indeices{ 0,1,2,2,3,0 };
Shape UI::shape{ vertices,normals,textures,indeices };

void UI::Init_buffer() {
	shape.init_buffer();
}
void UI::load(const string& img_name, const char* sound_name, bool loop, float volume) {
	init_texture(texture, img_name, false);
	sound.reset(new MySound(sound_name, loop, volume));
}

void UI::draw() const {
	glBindVertexArray(shape.VAO);
	auto view_location = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	auto proj_location = glGetUniformLocation(shaderProgramID, "proj");
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	auto world_location = glGetUniformLocation(shaderProgramID, "world");
	glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(world));

	auto tiworld_location = glGetUniformLocation(shaderProgramID, "normalWorld");
	glUniformMatrix4fv(tiworld_location, 1, GL_FALSE, glm::value_ptr(world));

	auto light_location = glGetUniformLocation(shaderProgramID, "lightPos");
	glUniform3f(light_location, 0, 0, 0);

	auto light_color = glGetUniformLocation(shaderProgramID, "lightColor");
	glUniform3f(light_color, 1, 1, 1);

	auto color_loc = glGetUniformLocation(shaderProgramID, "color");
	glUniform4f(color_loc, 1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, shape.indeices.size(), GL_UNSIGNED_INT, 0);
}

void UI::chage_img(const string& new_img_name) {
	int widthImg, heightImg, numberOfChannel;
	unsigned char* data = my_load_image(new_img_name.c_str(), &widthImg, &heightImg, &numberOfChannel);

	if (data != NULL) {
		glBindTexture(GL_TEXTURE_2D, texture);

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

void UI::resize(float sx, float sy, float sz) {
	world = world * glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz));
}

void UI::move(float dx, float dy, float dz) {
	world = glm::translate(glm::mat4(1), glm::vec3(dx, dy, dz)) * world;
}

bool UI::isIn(float x, float y) {
	auto lb = world * glm::vec4(-1, -1, 0, 1), rt = world * glm::vec4(1, 1, 0, 1);
	if (lb.x <= x && x <= rt.x && lb.y <= y && y <= rt.y) {
		return true;
	}
	return false;
}

void UI::play_sound() {
	sound->play();
}

void UI::stop_sound() {
	sound->stop();
}