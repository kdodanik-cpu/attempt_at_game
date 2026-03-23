//
// Created by Krish on 3/7/2026.
//

#ifndef ATTEMPT_AT_GAME_PLAYER_H
#define ATTEMPT_AT_GAME_PLAYER_H
#include "raylib.h"
#include "world/World.h"

struct Player {
  Vector3 position;
  Vector3 velocity;
  float vertical_speed = 0.0f; // Added it because I needed to separate XZ movement from Y movement
  float yaw = 0.0f;
  float pitch = 0.0f;

  void update(float dt, const World& world);
  void handle_mouse_movement();
  Camera get_camera() const;
};

#endif // ATTEMPT_AT_GAME_PLAYER_H
