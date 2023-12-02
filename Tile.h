#pragma once

struct Indices {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
};

struct MeshData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    Indices indices;
};

class Tile {
private:
    std::vector<glm::vec3> vertices;
    Indices indices;
    GLfloat x = 0, y = 0, z = 0;
    GLfloat width = 0.2f, depth = 0.2f, height = 0.02f;

    GLuint VAO, VBO[2], EBO;
    GLuint shaderProgramID;

public:
    Tile(GLfloat _x, GLfloat _y, GLfloat _z, GLuint _shaderProgramID);

    void InitBuff();
    void Draw();
    bool isthereTile(glm::vec3 cube_floor_center);
    glm::vec3 GetTileCenter();
};

