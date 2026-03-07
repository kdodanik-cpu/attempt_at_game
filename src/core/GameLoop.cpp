//
// Created by Krish on 3/7/2026.
//
#include "GameLoop.h"
#include <raylib.h>
#include <raymath.h>

void GameLoop::Init() {
  DisableCursor();
}

void GameLoop::Update(Player& player) {
  float dt = GetFrameTime();
  player.Update(dt);
  player.HandleMouseMovement();

}

void GameLoop::Draw(const Player& player) {
  BeginDrawing();
  ClearBackground(WHITE);
  BeginMode3D(player.GetCamera());
  DrawPlane(
    Vector3{0.0f, 0.0f, 0.0f}, // Ground center coords
    Vector2{50, 50}, // Size of plane
    DARKGRAY);

  DrawCapsule(
    player.position,
    player.position + Vector3{0, 1.6f, 0},
    0.5f,
    8,
    8,
    PURPLE
    );
  EndMode3D();
  EndDrawing();
}