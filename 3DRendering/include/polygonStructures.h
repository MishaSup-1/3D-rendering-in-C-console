#include <stdbool.h>

/*
 * ты чо офигел? Я для кого делал инструмент?
 * зачем тебе код если есть уже иснтрументы для его?
 * ты так не шути :(
 */

struct Vertex2D {
	int x, y;
	float z;
};

struct Vertex3D {
	float x, y, z, w;
};

struct Polygon {
	int vert1, vert2, vert3;
	char color;
};

struct Mesh {
	struct Vertex3D* vertices;
	int countVertices;
	struct Vertex3D* verticesTransformed;
	
	struct Polygon* polygons;
	int countPolygons;
	
	struct Vertex3D pivot;
	struct Vertex3D position;
	float rotationX, rotationY, rotationZ;
	float scale;
	bool transparancy;
};

struct Camera {
	float x, y, z;
	int FOV[2];
	float rotationX, rotationY, rotationZ;
};
