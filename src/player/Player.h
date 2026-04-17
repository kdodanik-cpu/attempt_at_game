//
// Created by Krish on 3/7/2026.
//

#ifndef ATTEMPT_AT_GAME_PLAYER_H
#define ATTEMPT_AT_GAME_PLAYER_H
#include "raylib.h"
#include "world/World.h"
#include "world/Enemy.h"
#include "player/Gun.h"

struct Player {
  Vector3 position;
  Vector3 velocity;
  float vertical_speed = 0.0f; // Added it because I needed to separate XZ movement from Y movement
  float yaw = 0.0f;
  float pitch = 0.0f;
  Gun gun;
  Vector3 last_hit_point = {0};
  float hit_marker_timer = 0;

  void update(float dt, const World& world, std::vector<Enemy>& enemies);
  void handle_mouse_movement();
  Camera get_camera() const;
  void shoot(
    const World& world,
    std::vector<Enemy>& enemies );
};

#endif // ATTEMPT_AT_GAME_PLAYER_H
