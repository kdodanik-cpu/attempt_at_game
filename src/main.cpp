
#include <raylib.h>
#include "core/GameLoop.h"

int main() {
  InitWindow(1280, 720, "attempt_at_game");
  SetTargetFPS(60);
  Player player;
  player.position = Vector3(0,0.25f, -2.0f);
  player.velocity = Vector3(0,0,0);
  GameLoop::Init();
  while (!WindowShouldClose()) {
    GameLoop::Update(player);
    GameLoop::Draw(player);
  }
  CloseWindow();
  return 0;
}