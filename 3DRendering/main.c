#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include "include/polygonStructures.h"
#include "mainEngine.h"

int SCREENSIZEX = 119 + 1, SCREENSIZEY = 29; /*119 + 1: 119 width screen, 1 for '\0'*/
struct Camera camera = {0.0f, 0.0f, 0.0f, {90, 126}, 0.0f, 0.0f, 0.0f};

void drawScreen(char pixels[SCREENSIZEY][SCREENSIZEX]);

#define TO_RAD (3.14159/180.0f)

int main () {
	char screen[SCREENSIZEY][SCREENSIZEX];
	memset(screen, ' ', sizeof(screen));
	
	struct Mesh model;
	struct Vertex3D vertices[] = {
		{-1.0f, -1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, -1.0f, 1.0f},
		{-1.0f, -1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, -1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f, 1.0f}
	};
	struct Polygon Polygons[] = {
		{0, 1, 3, '*'},
		{1, 3, 6, '#'},
		{0, 2, 5, '*'},
		{2, 5, 3, '#'},
		{0, 2, 1, '*'},
		{2, 1, 7, '#'},
		{1, 6, 7, '*'},
		{6, 7, 4, '#'},
		{7, 4, 2, '*'},
		{4, 2, 5, '#'},
		{5, 3, 4, '*'},
		{3, 4, 6, '#'}
		};
	model.vertices = vertices;
	model.countVertices = 8;
	model.polygons = Polygons;
	model.countPolygons = 12;
	struct Vertex3D pivot = {0.0f, 0.0f, 0.0f};
	struct Vertex3D position = {0.0f, 0.0f, 3.0f};
	model.pivot = pivot;
	model.position = position;
	model.rotationX = 0.0f;
	model.rotationY = 0.0f;
	model.rotationZ = 0.0f;
	model.scale = 1.0f;
	int modelsID = 0;
	struct Mesh models[1000];
	models[modelsID] = model;
	modelsID++;
	
	int running = 1;
	while(running) {
		Sleep(100);
		memset(screen, ' ', sizeof(screen));
		models[0].rotationY += 5;
		if(models[0].rotationY > 360) {
			models[0].rotationY = 0.0f;
		} else if(models[0].rotationY < 0) {
			models[0].rotationY = 360.0f;
		}

		render(models, SCREENSIZEX, SCREENSIZEY, screen, camera);
		system("cls");
		drawScreen(screen);
	}
	return 0;
}

void drawScreen(char pixels[SCREENSIZEY][SCREENSIZEX]) {
	for(int di = 0; di < SCREENSIZEY; di++) {
		printf("%s\n", pixels[di]);
	}
}
