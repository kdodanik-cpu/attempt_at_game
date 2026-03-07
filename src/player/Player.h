//
// Created by Krish on 3/7/2026.
//

#ifndef ATTEMPT_AT_GAME_PLAYER_H
#define ATTEMPT_AT_GAME_PLAYER_H
#include "raylib.h"

struct Player {
  Vector3 position;
  Vector3 velocity;

  void Update(float dt);
};

#endif // ATTEMPT_AT_GAME_PLAYER_H
