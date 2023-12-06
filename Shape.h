#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


using namespace std;
extern GLuint shaderProgramID;

struct Vector3 {
	float x{ 0 }, y{ 0 }, z{ 0 };
};

struct Vector2 {
	float x{ 0 }, y{ 0 };
};

struct Shape {
	GLuint VBO, VAO, EBO, NBO, TBO;
	vector<Vector3> vertices;
	vector<Vector3> normals;
	vector<Vector2> textures;
	vector<int> indeices;
	Shape(const vector<Vector3>& v, const vector<Vector3>& n, const vector<Vector2>& t, const vector<int>& i) {
		vertices = v;
		normals = n;
		textures = t;
		indeices = i;
	}
	Shape(const char* file) {
		load_obj(file);
	}
	void init_buffer() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &NBO);
		glGenBuffers(1, &TBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(Vector2), textures.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeices.size() * sizeof(int), indeices.data(), GL_STATIC_DRAW);
	}
	void load_obj(const char* filePath) {
		ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filePath << std::endl;
			return;
		}

		string line;
		map<string, int> tempMap;
		map<int, string> inverseTempMap;
		vector<Vector3> tempV, tempN;
		vector<Vector2> tempT;
		while (std::getline(file, line)) {
			istringstream iss(line);
			string token;
			iss >> token;

			if (token == "v") {
				// 정점 데이터
				Vector3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				tempV.push_back(vertex);
			}
			else if (token == "vn") {
				// 노말 데이터
				Vector3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				tempN.push_back(normal);
			}
			else if (token == "vt") {
				// 텍스쳐 좌표
				Vector2 texture;
				iss >> texture.x >> texture.y;
				tempT.emplace_back(texture);
			}
			else if (token == "f") {
				// 인덱스 데이터
				string faceData;
				getline(iss, faceData);
				istringstream faceIss(faceData);
				string space;
				getline(faceIss, space, ' ');
				for (int i = 0; i < 3; ++i) {
					string vertexIndexStr, texCoordIndexStr, normalIndexStr;

					getline(faceIss, vertexIndexStr, '/');
					getline(faceIss, texCoordIndexStr, '/');
					getline(faceIss, normalIndexStr, ' ');

					string key = vertexIndexStr + '/' + texCoordIndexStr + '/' + normalIndexStr + ' ';
					tempMap.emplace(key, tempMap.size());
					inverseTempMap.emplace(tempMap[key], key);
					indeices.push_back(tempMap[key]);
				}
			}
		}

		for (const auto& p : inverseTempMap) {
			istringstream iss(p.second);
			string vertexIndexStr, texCoordIndexStr, normalIndexStr;
			getline(iss, vertexIndexStr, '/');
			getline(iss, texCoordIndexStr, '/');
			getline(iss, normalIndexStr, ' ');
			vertices.emplace_back(tempV[stoi(vertexIndexStr) - 1]);

			textures.emplace_back(tempT[stoi(texCoordIndexStr) - 1]);

			normals.emplace_back(tempN[stoi(normalIndexStr) - 1]);
		}

		file.close();
	}
};