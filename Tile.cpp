#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>

#include "Tile.h"

using namespace std;


MeshData Load_Object_Tile(const char* path)	// obj ���� �д� �Լ�
{
	MeshData meshData;

	meshData.indices.vertexIndices.clear();
	meshData.indices.uvIndices.clear();
	meshData.indices.normalIndices.clear();
	meshData.vertices.clear();
	meshData.uvs.clear();
	meshData.normals.clear();

	ifstream in(path);
	if (!in) {
		cerr << path << "���� ��ã��";
		exit(1);
	}


	while (in) {
		string lineHeader;
		in >> lineHeader;
		if (lineHeader == "v") {
			glm::vec3 vertex;
			in >> vertex.x >> vertex.y >> vertex.z;
			meshData.vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			in >> uv.x >> uv.y;
			meshData.uvs.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			in >> normal.x >> normal.y >> normal.z;
			meshData.normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			char a;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			for (int i = 0; i < 3; i++)
			{
				in >> vertexIndex[i] >> a >> uvIndex[i] >> a >> normalIndex[i];
				meshData.indices.vertexIndices.push_back(vertexIndex[i] - 1);
				meshData.indices.uvIndices.push_back(uvIndex[i] - 1);
				meshData.indices.normalIndices.push_back(normalIndex[i] - 1);
			}
		}
	}

	return meshData;
}

// --------- Tile
void Tile::InitBuff()
{
	MeshData tileData = Load_Object_Tile("tile.obj");
	vertices = tileData.vertices;
	indices = tileData.indices;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.vertexIndices.size() * sizeof(GLuint), &indices.vertexIndices[0], GL_STATIC_DRAW);

	// �÷� ���� VBO...
	//glGenBuffers(1, &VBO[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);


    glBindVertexArray(0);
}

void Tile::Draw()
{
	glm::mat4 TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(x, y, z));	// Ŭ���� ���� �� ������ ��ġ�� �̵�
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "TileTransform");	// ������ ���� TileTransform ���??
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glBindVertexArray(VAO);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 36, 40);

	//glutSwapBuffers();
}

glm::vec3 Tile::GetTileCenter()
{
	return glm::vec3(x, 0.f, z);
}

bool Tile::isthereTile(glm::vec3 cube_floor_center) 
{
	GLfloat tileLeft = x - width / 2.0f;
	GLfloat tileRight = x + width / 2.0f;
	GLfloat tileTop = z + height / 2.0f;
	GLfloat tileBottom = z - height / 2.0f;

	if (cube_floor_center.y >= 0) {
		if (cube_floor_center.x >= tileLeft && cube_floor_center.x <= tileRight &&
			cube_floor_center.z >= tileBottom && cube_floor_center.z <= tileTop) {	// �ε��Ҽ��� �񱳿��� �̽��� ���� ���� �� ��
			return true;	// Ÿ�� ���� ������ ����
		}
		else {
			return false;	// Ÿ�� �ۿ� ������ ����
		}
	}
	return false;
}
