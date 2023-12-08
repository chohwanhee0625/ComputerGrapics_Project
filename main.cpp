#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <memory>
#include "Tools.h"
#include "GameManager.h"

using namespace std;

ostream& operator<<(ostream& out, const glm::vec4& vec) {
	auto x = vec.x, y = vec.y, z = vec.z;
	//if (abs(x) <= 0.0001) {
	//	x = 0;
	//}
	//if (abs(y) <= 0.0001) {
	//	y = 0;
	//}
	//if (abs(z) <= 0.0001) {
	//	z = 0;
	//}
	out << x << ", " << y << ", " << z;
	return out;
}
ostream& operator<<(ostream& out, const glm::vec3& vec) {
	auto x = vec.x, y = vec.y, z = vec.z;
	//if (abs(x) <= 0.000001) {
	//	x = 0;
	//}
	//if (abs(y) <= 0.000001) {
	//	y = 0;
	//}
	//if (abs(z) <= 0.000001) {
	//	z = 0;
	//}
	out << x << ", " << y << ", " << z;
	return out;
}

int width, height;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShader();
void make_fragmentShader();
GLuint make_shaderProgram();

GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

void keyboard(unsigned char, int, int);
void special_key(int, int, int);
void mouse(int, int, int, int);
extern GameManager GM;

void main(int argc, char** argv) {
	width = 800;
	height = 600;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW Initialized\n";
	}
	glewExperimental = GL_TRUE;
	glewInit();
	make_vertexShader();
	make_fragmentShader();
	shaderProgramID = make_shaderProgram();
	GM.load_game();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(10, timer, 0);
	glutSpecialFunc(special_key);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
GLvoid drawScene() {
	GM.render();
}
GLvoid Reshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}
void make_vertexShader() {
	GLchar* vertexSource;
	vertexSource = filetobuf("shader/vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader error\n" << errorLog << std::endl;
		return;
	}

}
void make_fragmentShader() {
	GLchar* fragmentSource = filetobuf("shader/fragment.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram() {
	GLuint shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(shaderID);

	return shaderID;
}
void keyboard(unsigned char key, int x, int y) {
	GM.handle_key(key);
}
void special_key(int key, int x, int y) {
	GM.handle_special_key(key);
}

void mouse(int button, int state, int x, int y) {
	float glX, glY;
	window2GL(x, y, width, height, glX, glY);
	GM.handle_mouse(button, state, glX, glY);
}