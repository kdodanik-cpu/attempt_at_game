//
// Created by Krish on 3/7/2026.
//
#include "GameLoop.h"

#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include "world/World.h"
#include "world/Enemy.h"

static World world;
static std::vector<Enemy> enemies;

void GameLoop::init() {
  DisableCursor();
  world.init();
  Enemy test_enemy;
  test_enemy = {
    1.6f,
    0.5f,
    Vector3{0.0f, 0.0f, 2.0f},
    100
  };
  enemies.push_back(test_enemy);
}

void GameLoop::update(Player& player) {
  float dt = GetFrameTime();
  player.update(dt, world, enemies);
  player.handle_mouse_movement();
  enemies.erase(
    std::remove_if(
      enemies.begin(), enemies.end(),
      [](const Enemy& e){return !e.is_alive();}),
  enemies.end());
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
  for (const Enemy& enemy : enemies)
  {
      DrawCapsule(
        enemy.position,
        Vector3{
          enemy.position.x,
          enemy.position.y + enemy.height,
          enemy.position.z},
          enemy.radius,
          8,
          8,
          YELLOW
        );
  }
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