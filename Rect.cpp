#include "Rect.h"
static vector<Vector3> vertices{ {-1,0,1},{-1,0,-1},{1,0,-1},{1,0,1} };
static vector<Vector3> normals{ {0,1,0},{0,1,0 },{0,1,0} ,{0,1,0} };
static vector<Vector2> textures{ {1,1},{1,0},{0,0},{0,1} };
static vector<int> indeices{ 0,1,2,2,3,0 };
Shape Rect::shape{ vertices,normals,textures,indeices };
