#pragma once
#include "Shape.h"
#include "light.h"
#include "Cube.h"

class Tile {
    unsigned int texture;
    glm::mat4 world, normal_world;
    glm::mat4 S{ glm::scale(glm::mat4(1),glm::vec3(1,-0.3,1)) }, T{ 1 }, R{ 1 };

protected:
    glm::vec4 color{ 1,1,1,1 };
    string img_name{ "texture/tile.png" };
    static Shape shape;
    void init_texture();
    float tick = -0.5;

public:
    Tile(float x, float z);
    Tile() = delete;
    void load();
    virtual void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const;

    void update_world();
    virtual void handle_collision(Cube& cube);
    void goal_animation();

    glm::vec3 get_lb() const;
    glm::vec3 get_rt() const;
};

