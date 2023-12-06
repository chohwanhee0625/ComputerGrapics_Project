#include "Tile.h"

using namespace std;

Shape Tile::shape{ "cube.obj" };

void Tile::init_buffer() {
	shape.init_buffer();
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

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(int)));
	}
}

void Tile::update_world() {
	world = T * R * S;
	normal_world = glm::transpose(glm::inverse(world));
}

void Tile::resize(float sx, float sy, float sz) {
	S = glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz));
	update_world();
}

glm::vec3 Tile::GetTileCenter()
{
	return glm::vec3(x, 0.f, z);
}

bool Tile::isthereTile(glm::vec3 lt, glm::vec3 rb)
{
	GLfloat tileLeft = tile_center.x - width / 2.0f;
	GLfloat tileRight = x + width / 2.0f;
	GLfloat tileTop = z + height / 2.0f;
	GLfloat tileBottom = z - height / 2.0f;

	if (cube_floor.x >= tileLeft && cube_floor.x <= tileRight &&
		cube_floor.z >= tileBottom && cube_floor.z <= tileTop) {	// 부동소수점 비교연산 이슈로 범위 지정 후 비교
			return true;	// 타일 위에 놓여진 상태
	}
	if ()

	return false;
	// 타일은 좌표 하나에 대해 bool 값만 리턴, GM은 tile.istherTile(cube.lt), (rb)
	// 두 번 호출, 하나라도 false 뜨면 cube.fall 실행
}
