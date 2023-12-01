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

typedef struct {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
}Indices;

typedef struct {
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	Indices indeces;
}MeshData;


MeshData Load_Object_Tile(const char* path)	// obj 파일 읽는 함수
{
	MeshData meshData;

	meshData.indeces.vertexIndices.clear();
	meshData.indeces.uvIndices.clear();
	meshData.indeces.normalIndices.clear();
	meshData.vertices.clear();
	meshData.uvs.clear();
	meshData.normals.clear();

	ifstream in(path);
	if (!in) {
		cerr << path << "파일 못찾음";
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
				meshData.indeces.vertexIndices.push_back(vertexIndex[i] - 1);
				meshData.indeces.uvIndices.push_back(uvIndex[i] - 1);
				meshData.indeces.normalIndices.push_back(normalIndex[i] - 1);
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
	indices = tileData.indeces;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.vertexIndices.size() * sizeof(GLuint), &indices.vertexIndices[0], GL_STATIC_DRAW);

	// 컬러 관련 VBO...
	//glGenBuffers(1, &VBO[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);


    glBindVertexArray(0);
}

void Tile::Draw()
{
	glm::mat4 TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(x, y, z));	// 클래스 생성 시 지정한 위치로 이동
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "TileTransform");	// 유니폼 변수 TileTransform 사용??
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glBindVertexArray(VAO);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 36, 40);

	//glutSwapBuffers();
}

bool Tile::CollisionCheck(GLuint cubeVBO, GLuint cubeVerticesCount) {
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLfloat* cubeVertices = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);	// Cube 의 현재 위치를 cube의 vbo 에서 읽어옴
	if (cubeVertices == nullptr) {
		std::cerr << "Failed to map cube vertices buffer" << std::endl;
		// exit(-1);
		return false;
	}

	GLfloat tileLeft = x - width / 2.0f;
	GLfloat tileRight = x + width / 2.0f;
	GLfloat tileFront = y - depth / 2.0f;
	GLfloat tileBack = y + depth / 2.0f;
	GLfloat tileTop = z + height / 2.0f;
	GLfloat tileBottom = z - height / 2.0f;

	for (int i = 0; i < cubeVerticesCount; i += 3) {	// 삼각형 매쉬의 각 정점 정보
		GLfloat vertexX = cubeVertices[i];
		GLfloat vertexY = cubeVertices[i + 1];
		GLfloat vertexZ = cubeVertices[i + 2];

		if (vertexY <= 0.f) {	// 큐브가 서 있을때?
			if (vertexX >= tileLeft && vertexX <= tileRight &&
				vertexZ >= tileBottom && vertexZ <= tileTop) {
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				return true;	// 타일 위에 놓여진 상태
			}
			else {
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				return false;	// 타일 밖에 놓여진 상태
			}
		}
	}
}

