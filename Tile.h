#pragma once
#include "Cube.h"
#include "Tools.h"

class Tile {
    glm::mat4 world, normal_world;
    glm::mat4 S{ glm::scale(glm::mat4(1),glm::vec3(1,-0.3,1)) }, T{ 1 }, R{ 1 };

protected:
    glm::vec4 color{ 1,1,1,1 };
    string img_name{ "texture/tile.png" };
    unsigned int* texture;
    static Shape shape;
    float tick = -0.5;

public:
    Tile(float x, float z);
    Tile() = delete;
    virtual void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& eye, const Light& light) const;
    static void load();
    void update_world();
    virtual void handle_collision(Cube& cube);
    void goal_animation();

    glm::vec3 get_lb() const;
    glm::vec3 get_rt() const;
};

