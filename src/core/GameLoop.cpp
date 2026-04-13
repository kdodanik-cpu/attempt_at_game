//
// Created by Krish on 3/7/2026.
//
#include "GameLoop.h"
#include <raylib.h>
#include <raymath.h>
#include "world/World.h"

static World world;

void GameLoop::init() {
  DisableCursor();
  world.init();
}

void GameLoop::update(Player& player) {
  float dt = GetFrameTime();
  player.update(dt, world);
  player.handle_mouse_movement();
}

void GameLoop::draw(const Player& player) {
  BeginDrawing();
  ClearBackground(WHITE);
  BeginMode3D(player.get_camera());
  world.draw();
  DrawCapsule(
    player.position,
    player.position + Vector3{0, 1.6f, 0},
    0.5f,
    8,
    8,
    PURPLE
    );

  if (player.hit_marker_timer > 0)
    DrawSphere(player.last_hit_point, 0.1f, RED);

  EndMode3D();
  // Horizontal line
  DrawLine(
    GetScreenWidth() * 0.5f - 15.0f,
    GetScreenHeight() * 0.5f,
    GetScreenWidth() * 0.5f + 15.0f,
    GetScreenHeight() * 0.5f,
    BLACK
    );
  // Vertical line
  DrawLine(
    GetScreenWidth() * 0.5f,
    GetScreenHeight() * 0.5f - 15.0f,
    GetScreenWidth() * 0.5f,
    GetScreenHeight() * 0.5f + 15.0f,
    BLACK);

  EndDrawing();
}