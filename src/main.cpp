#include <raylib.h>

int main() {
  InitWindow(1280, 720, "attempt_at_game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("it works", 40, 40, 20, WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}