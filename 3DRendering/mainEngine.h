#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "include/PolygonsFunctions.h"

float nSinf(float x) {return -sinf(x);}
float nCosf(float x) {return -cosf(x);}

float constNullF(float x) {return 0.0f;}
float constOneF(float x) {return 1.0f;}

#define TO_RAD (3.14159/180.0f)

// rotation object matrix
MathFunc ROMX[][4] = {
	{constOneF, constNullF, constNullF, constNullF},
	{constNullF, cosf, nSinf, constNullF},
	{constNullF, sinf, cosf, constNullF},
	{constNullF, constNullF, constNullF, constOneF}
};
MathFunc ROMY[][4] = {
	{cosf, constNullF, sinf, constNullF},
	{constNullF, constOneF, constNullF, constNullF},
	{nSinf, constNullF, cosf, constNullF},
	{constNullF, constNullF, constNullF, constOneF}
};
MathFunc ROMZ[][4] = {
	{cosf, nSinf, constNullF, constNullF},
	{sinf, cosf, constNullF, constNullF},
	{constNullF, constNullF, constOneF, constNullF},
	{constNullF, constNullF, constNullF, constOneF}
};

void render(struct Mesh models[], int SCREENSIZEX, int SCREENSIZEY, char screen[SCREENSIZEY][SCREENSIZEX], struct Camera camera) {
	float zBufer[SCREENSIZEX][SCREENSIZEY];
	for(int iy = 0; iy < SCREENSIZEY; iy++) {
		for(int ix = 0; ix < SCREENSIZEX; ix++) {
			zBufer[iy][ix] = 1e30f;
		}
	}
	for(int iMain = 0; iMain < 200; iMain++) {
		struct Mesh model = models[iMain];
		struct Vertex3D vertTransformed[model.countVertices];
		model.verticesTransformed = vertTransformed;
		if(model.countVertices <= 0 || !model.transparancy) {
			for(int i = 0; i < model.countVertices; i++) {
				float input[] = {((model.vertices[i].x - model.pivot.x) * model.scale),
					((model.vertices[i].y - model.pivot.y) * model.scale),
					((model.vertices[i].z - model.pivot.z) * model.scale),
					model.vertices[i].w
					 };
				
				float modelAngle[] = {
					model.rotationX * TO_RAD,
					model.rotationY * TO_RAD,
					model.rotationZ * TO_RAD
				};
				
				float cameraAngle[] = {
					-camera.rotationX * TO_RAD,
					-camera.rotationY * TO_RAD,
					-camera.rotationZ * TO_RAD
				};
				
				MathFunc (*allMatrix[])[4] = {ROMX, ROMY, ROMZ};
				
				int iterations = 3;
				
				for(int mi = 0; mi < iterations; mi++) {
					matrixRange(modelAngle[mi], input, (float*)&model.verticesTransformed[i], allMatrix[mi]);
					input[0] = model.verticesTransformed[i].x;
					input[1] = model.verticesTransformed[i].y;
					input[2] = model.verticesTransformed[i].z;
					input[3] = model.verticesTransformed[i].w;
				}
				
				model.verticesTransformed[i].x += model.position.x + model.pivot.x;
				model.verticesTransformed[i].y += model.position.y + model.pivot.y;
				model.verticesTransformed[i].z += model.position.z + model.pivot.z;
				
				model.verticesTransformed[i].x -= camera.x;
				model.verticesTransformed[i].y -= camera.y;
				model.verticesTransformed[i].z -= camera.z;
				
				input[0] = model.verticesTransformed[i].x;
				input[1] = model.verticesTransformed[i].y;
				input[2] = model.verticesTransformed[i].z;
				input[3] = model.verticesTransformed[i].w;
				
				for(int mi = 0; mi < iterations; mi++) {
					matrixRange(cameraAngle[mi], input, (float*)&model.verticesTransformed[i], allMatrix[mi]);
					input[0] = model.verticesTransformed[i].x;
					input[1] = model.verticesTransformed[i].y;
					input[2] = model.verticesTransformed[i].z;
					input[3] = model.verticesTransformed[i].w;
				}
			}
			
			struct Vertex2D verticesScreen[model.countVertices] = {};
			for(int i = 0; i < model.countVertices; i++) {
				int sx = (model.verticesTransformed[i].x / model.verticesTransformed[i].z) * (1 / tanf((camera.FOV[0] * TO_RAD)/2) / ((float)SCREENSIZEX/(float)SCREENSIZEY)) * (SCREENSIZEX / 2) + (SCREENSIZEX / 2);
				int sy = (model.verticesTransformed[i].y / model.verticesTransformed[i].z) * (1 / (tanf((camera.FOV[1] * TO_RAD)/2))) * -(SCREENSIZEY / 2) + (SCREENSIZEY / 2);
				verticesScreen[i].x = sx;
				verticesScreen[i].y = sy;
				verticesScreen[i].z = model.verticesTransformed[i].z;
			}
			
			for(int i = 0; i < model.countPolygons; i++) {
				struct Polygon poly = model.polygons[i];
				for(int y = 0; y < SCREENSIZEY; y++) {
					for(int x = 0; x < SCREENSIZEX; x++) {
						int pixel[] = {x, y};
						if(inPolygon(verticesScreen[poly.vert1], verticesScreen[poly.vert2], verticesScreen[poly.vert3], pixel)) {
							float zPixel = cForZBuffer(verticesScreen[poly.vert1], verticesScreen[poly.vert2], verticesScreen[poly.vert3], pixel);
							if(zPixel < zBufer[y][x] && zPixel > 0) {
								screen[y][x] = poly.color;
								zBufer[y][x] = zPixel;
							}
						}
					}
				}
			}
		}
	}
	for(int c = 0; c < SCREENSIZEY; c++) {
		screen[c][SCREENSIZEX - 1] = '\0';
	}
}
