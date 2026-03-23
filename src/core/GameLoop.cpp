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
  EndMode3D();
  EndDrawing();
}