#pragma once
#include "Shape.h"
struct Light {
	glm::vec3 color{ 1,1,1 };
	glm::vec3 pos{ 0,100,0 };
	Light(const glm::vec3& pos) : pos{ pos } {}

	void lighting() const {
		auto light_location = glGetUniformLocation(shaderProgramID, "lightPos");
		glUniform3fv(light_location, 1, glm::value_ptr(pos));
		auto light_color = glGetUniformLocation(shaderProgramID, "lightColor");
		glUniform3fv(light_color, 1, glm::value_ptr(color));
	}
};