
#include <raylib.h>
#include "core/GameLoop.h"

extern Camera camera;

int main() {
  InitWindow(1280, 720, "attempt_at_game");
  SetTargetFPS(60);
  Player player;
  player.position = Vector3(0,1.5f, -2.0f);
  player.velocity = Vector3(0,0,0);
  while (!WindowShouldClose()) {
    GameLoop::Update(player, camera);
    GameLoop::Draw(player);
  }
  CloseWindow();
  return 0;
}