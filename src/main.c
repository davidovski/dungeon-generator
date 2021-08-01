#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"

#include "mst.h"
#include "const.h"

const int screenWidth = 800;
const int screenHeight = 800;
const int seed = 123456781;

const int radius = 20;

typedef struct R {
	int x;
	int y;
	int w;
	int h;
} Room;

// In this example, the idea is that rooms will be made up of tiles in a game
// 1 pixel/unit in this algorithm is intended to 

Room rooms[ROOM_COUNT];

// define each corridor as a branch between a room and its "parent"
int *parentRooms;

// extend build in C random function to return a value from 0.0d to 1.0d
double randomDouble() {
	return rand() / (double)(RAND_MAX);
}


// Randomly position rooms around the map, spanning around a circular space
void positionRooms() {
	for (int i = 0; i < ROOM_COUNT; i++) {
		double t = 2 * PI * randomDouble();
		double u = 2 * randomDouble();
		double r;
		if (u > 1) {
			r = 2-u;
		} else {
			r = u;
		}

		int x = (screenWidth/2) + radius*r*cos(t);
		int y = (screenHeight/2) + radius*r*sin(t);
		int w = MIN_SIZE + randomDouble() * (MAX_SIZE-MIN_SIZE);
		int h = MIN_SIZE + randomDouble() * (MAX_SIZE-MIN_SIZE);

		rooms[i] = (Room){x, y, w, h};
	}
} 

// Determine if two rooms are colliding with each other or not.
// true: colliding
// false: not colliding
bool areColliding(Room a, Room b) {
	bool h1 = (b.x < a.x+a.w);
	bool h2 = (b.x+b.w > a.x);

	bool v1 = (b.y < a.y+a.h);
	bool v2 = (b.y+b.h > a.y);

	return h1 && h2 && v1 && v2;
}


void separateRooms() {

	// Move the rooms away from each other until there are no collisions left
	bool d = true;
	while (d) {
		// set d to false. If no rooms are moved, then we are done with this
		d = false;

		// iterate through all rooms and check if all other rooms are colliding with each room
		for (int i = 0; i < ROOM_COUNT; i++) {
			for (int j = 0; j < ROOM_COUNT; j++) {
				// only check if they are different rooms
				if (j != i) {
					// only move it if they are colliding
					if (areColliding(rooms[i], rooms[j])) {
						d = true;
						Room *room1 = &rooms[i];
						Room *room2 = &rooms[j];
						
						// calculate the difference in centers of the two rooms
						int c1x = room1->x + (room1->w/2);
						int c1y = room1->y + (room1->h/2);

						int c2x = room2->x + (room2->w/2);
						int c2y = room2->y + (room2->h/2);

						int dx = c1x - c2x;
						int dy = c1y - c2y;

						// move the current room away from the other room by the difference of centers + half the width of the room
						room1->x += dx;
						room1->y += dy; 
						
						room1->x += (abs(dx)/dx) * (room1->w/2);
						room1->y += (abs(dy)/dy) * (room1->h/2);

					}
				}
			}
		}
	}
}

// function to get the center of each room as a Vector2 point.
Vector2 getCenter(Room *room) {
	return (Vector2){room->x+(room->w/2), room->y+(room->h/2)};
}


void mapCorridors() {
	// Make things easier for us by working with only the center points of each room
	Vector2 nodes[ROOM_COUNT];
	for (int i = 0; i < ROOM_COUNT; i++) {
		nodes[i] = getCenter(&rooms[i]);
	}

	// Map the rooms points into an adjacency matrix  
	int graph[ROOM_COUNT][ROOM_COUNT];
	

	for (int i = 0; i < ROOM_COUNT; i++) {
		for (int j = 0; j < ROOM_COUNT; j++) {
			// we only really need to compare distances between nodes, so to save time, we will leave distances squared
			int dx = (nodes[i].x - nodes[j].x);
			int dy = (nodes[i].y - nodes[j].y);
			int distanceSquared = dx*dx + dy*dy;
			graph[i][j] = distanceSquared;
		}
	}


	parentRooms = primMST(graph);
}


void createCorridors() {

}

void generateRooms() {
	positionRooms();
	separateRooms();
	mapCorridors();
	
}


int main(void) {
	srand(time(NULL));

	generateRooms();

	InitWindow(screenWidth, screenHeight, "dungeon generation");
	SetTargetFPS(20);

	while (!WindowShouldClose()) {
		
		BeginDrawing();
		ClearBackground(GRAY);

		for (int i = 0; i < ROOM_COUNT; i++) {
			for (int j = 0; j < ROOM_COUNT; j++) {
				Vector2 a = getCenter(&rooms[i]);
				Vector2 b = getCenter(&rooms[j]);

				DrawLine(a.x, a.y, b.x, b.y, RED);
			}
		}

		for (int i = 0; i < ROOM_COUNT; i++) {
			Color c;
			if (i == 0) c = BLACK;
			else	    c = LIGHTGRAY;
			DrawRectangleLines(rooms[i].x, rooms[i].y, rooms[i].w, rooms[i].h, c); 
			
			int p = parentRooms[i];

			if (p > -1 && p < ROOM_COUNT) {
				Vector2 a = getCenter(&rooms[i]);
				Vector2 b = getCenter(&rooms[p]);

				DrawLine(a.x, a.y, b.x, b.y, GREEN);
			}
		}

	

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
