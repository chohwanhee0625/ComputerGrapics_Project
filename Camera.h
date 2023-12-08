#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
struct Camera {
	float n = 0.1f, f = 1000.0f;
	float fovy = 90;
	float left{ -1 }, right{ 1 }, bottom{ -1 }, top{ 1 }, zNear{ 0.1 }, zFar{ 10 };
	glm::vec3 EYE = glm::vec3(1.f, 5.f, 1.f), AT = glm::vec3(0.f, 0.f, 0.f), UP = glm::vec3(0.f,1.f, 0.f);
	glm::mat4 proj;
	glm::mat4 view;
	Camera(const glm::vec3& EYE) :EYE{ EYE } {
		view = glm::lookAt(EYE, AT, UP);
		proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 1000.f);
	}
	Camera() {
		view = glm::lookAt(EYE, AT, UP);
		proj = glm::perspective(fovy, 1.f, n, f);
	}
	Camera(glm::vec3 EYE, glm::vec3 UP, glm::vec3 AT) :EYE{ EYE }, AT{ AT }, UP{ UP } {
		view = glm::lookAt(EYE, AT, UP);
		proj = glm::perspective(fovy, 1.f, n, f);
	}
	void set_view(const glm::vec3& e, const glm::vec3& a, const glm::vec3& u) {
		view = glm::lookAt(e, a, u);
		EYE = e;
		AT = a;
		UP = u;
	}
};