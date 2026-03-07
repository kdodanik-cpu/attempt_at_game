//
// Created by Krish on 3/7/2026.
//
#include "GameLoop.h"
#include <raymath.h>

Camera camera = {.position = Vector3(0, 2.0f, 3.0f),
  .target = Vector3{0, 0, 0},
  .up = {0, 1, 0},
  .fovy = 75.0f,
  .projection = CAMERA_PERSPECTIVE};

void GameLoop::Update(Player& player, Camera& camera) {
  float dt = GetFrameTime();
  player.Update(dt);
  camera.position = player.position + Vector3{0, 1.8f, 0.0f}; // Raise to eye-level
  camera.target = player.position + Vector3{0, 0.0f, -1.0f}; // Look to front*/
}

float GameLoop::GetDeltaTime() {
  return GetFrameTime();
}

void GameLoop::Draw(const Player& player) {
  BeginDrawing();
  ClearBackground(WHITE);
  BeginMode3D(camera);
  DrawPlane(
    Vector3{0.0f, 0.0f, 0.0f}, // Ground center coords
    Vector2{5, 5}, // Size of plane
    DARKGRAY);

  DrawCapsule(
    player.position,
    player.position + Vector3{-1.0f, -1.0f, -2.0f},
    0.5f,
    8,
    8,
    PURPLE
    );

  EndMode3D();
  EndDrawing();
}