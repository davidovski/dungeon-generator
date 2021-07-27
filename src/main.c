#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
	InitWindow(screenWidth, screenHeight, "dungeon generation");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		
		BeginDrawing();
		ClearBackground(GRAY);

		EndDrawing();

	}

	CloseWindow();

	return 0;
}
