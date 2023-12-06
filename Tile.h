#pragma once

#include "Shape.h"
#include "light.h"


class Tile {
private:
    static Shape shape;
    unsigned int texture[6];
    glm::mat4 world, normal_world, coord_space{ 1 };
    glm::mat4 S{ glm::scale(glm::mat4(1),glm::vec3(1,0.1,1)) }, T{ 1 }, R{ 1 };
    glm::vec4 color{ 1,1,1,1 };
    glm::mat4 world, normal_world, coord_space{ 1 };


    std::vector<glm::vec3> vertices;
    glm::vec3 tile_center = glm::vec3(0, 0, 0);
    GLfloat width = 1.0f, depth = 1.0f, height = 0.02f;

    GLuint VAO, VBO[2], EBO;
    GLuint shaderProgramID;

public:
    Tile(GLfloat _x, GLfloat _y, GLfloat _z);
    void init_buffer();
    void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const;
    void resize(float sx, float sy, float sz);
    void update_world();

    bool isthereTile(glm::vec3 cube_floor);
    glm::vec3 GetTileCenter();
};

