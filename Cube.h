#pragma once
#include "Shape.h"
#include "light.h"

#define BOTTOM_FACE 0
#define TOP_FACE 1
#define RIGHT_FACE 2
#define LEFT_FACE 3
#define FRONT_FACE 4
#define BACK_FACE 5

// 화면 안쪽 Front, 화면 왼쪽 Left
extern ostream& operator<<(ostream& out, const glm::vec4& vec);
extern ostream& operator<<(ostream& out, const glm::vec3& vec);
class Cube {
	static Shape shape;
	unsigned int texture[6];
	glm::mat4 world, normal_world, coord_space{ 1 };
	glm::mat4 S{ glm::scale(glm::mat4(1),glm::vec3(1,2,1)) }, T{ 1 }, R{ 1 };
	glm::vec4 color{ 1,1,1,1 };
	glm::vec4 faces[6];
	glm::vec3 x{ 1,0,0 }, y{ 0,1,0 }, z{ 0,0,1 };
	int floor_id = BOTTOM_FACE;
	map<string, glm::vec3> rotate_axis{
		{"FRONT",glm::vec3(-1,0,0)},
		{"BACK",glm::vec3(1,0,0)},
		{"LEFT",glm::vec3(0, 0, 1)},
		{"RIGHT",glm::vec3(0,0,-1)} };

	typedef glm::vec3(Cube::* myFunc)() const;
	map<string, myFunc> move_rule{
		{"FRONT", &Cube::get_front_edge},
		{"BACK", &Cube::get_back_edge},
		{"LEFT", &Cube::get_left_edge},
		{"RIGHT", &Cube::get_right_edge} };
	void init_texture();
public:
	Cube() {
		update_world();
		faces[BOTTOM_FACE] = glm::vec4{ 0,0,0,1 };
		faces[TOP_FACE] = glm::vec4{ 0,1,0,1 };
		faces[RIGHT_FACE] = glm::vec4{ 0.5,0.5,0,1 };
		faces[LEFT_FACE] = glm::vec4{ -0.5,0.5,0,1 };
		faces[FRONT_FACE] = glm::vec4{ 0,0.5,0.5,1 };
		faces[BACK_FACE] = glm::vec4{ 0,0.5,-0.5,1 };
	}
	void init_buffer();
	void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const;
	void update_world();
	void resize(float sx, float sy, float sz);
	void move(const string& dir);
	void fall();
	void change_axis(const string& dir);
	void check_floor_face();
	void slide(const glm::vec3& dir);
	glm::vec3 get_center() const;
	glm::vec4 get_floor_center() const;
	glm::vec3 get_floor_lt() const;
	glm::vec3 get_floor_rb() const;
	glm::vec3 get_front_edge() const;
	glm::vec3 get_back_edge() const;
	glm::vec3 get_left_edge() const;
	glm::vec3 get_right_edge() const;
};
