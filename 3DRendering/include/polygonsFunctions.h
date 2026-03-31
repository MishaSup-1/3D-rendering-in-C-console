#include "PolygonStructures.h"
#include "range.h"

float cForZBuffer(struct Vertex2D vert, struct Vertex2D vert1, struct Vertex2D vert2, int p[2]) {
	float denom = (vert1.x - vert.x) * (vert2.y - vert.y) - (vert2.x - vert.x) * (vert1.y - vert.y);
	if(denom == 0) {
		return 0.0f;
	}
	float a = ( (vert1.y - vert2.y) * (p[0] - vert2.x) - (vert1.x - vert2.x) * (p[1] - vert2.y) ) / denom;
	float b = ( (vert2.y - vert.y) * (p[0] - vert2.x) - (vert2.x - vert.x) * (p[1] - vert2.y) ) / denom;
	float c = 1.0f - a - b;
	if(a >= 0 && b >= 0 && c >= 0) {
		return a*vert.z + b*vert1.z + c*vert2.z;
	}
	return 0.0f;
}

bool inPolygon(struct Vertex2D vert, struct Vertex2D vert1, struct Vertex2D vert2, int pixel[2]) {
	struct Vertex2D p[3] = {vert, vert1, vert2};
	int minX = min3(vert.x, vert1.x, vert2.x);
	int maxX = max3(vert.x, vert1.x, vert2.x);
	int minY = min3(vert.y, vert1.y, vert2.y);
	int maxY = max3(vert.y, vert1.y, vert2.y);
	if(pixel[0] < minX || pixel[1] < minY || pixel[0] > maxX || pixel[1] > maxY) {
		return false;
	}
	for(int i = 0; i < 3; i++) {
		struct Vertex2D v1 = p[i], v2 = p[(i + 1) % 3], v3 = p[(i + 2) % 3];
		int t = (v2.x - v1.x) * (pixel[1] - v1.y) - (v2.y - v1.y) * (pixel[0] - v1.x);
		int t1 = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
		if(t && ! (t > 0 && t1 > 0) && ! (t < 0 && t1 < 0)) {
			return false;
		}
	}
	
	return true;
}

typedef float (*MathFunc)(float);

float matrixFormul(float angle, float a, MathFunc func) {
	float res = a * func(angle);
	return res;
}

void matrixRange(float angle, float* input, float* output, MathFunc matrix[4][4]) {
	for(int iMatrix1 = 0; iMatrix1 < 4; iMatrix1++) {
		float res = 0.0f;
		for(int iMatrix2 = 0; iMatrix2 < 4; iMatrix2++) {
			res += matrixFormul(angle, input[iMatrix2], matrix[iMatrix1][iMatrix2]);
		}
		output[iMatrix1] = res;
	}
}
