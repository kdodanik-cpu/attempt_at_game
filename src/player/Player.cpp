//
// Created by Krish on 3/7/2026.
//

#include "Player.h"
#include <raymath.h>

void Player::Update(float dt) {

  // Forward movement
  if (IsKeyDown(KEY_W)) {
    velocity.z = -5.0f;
  } else if (IsKeyDown(KEY_S)) {
    velocity.z = 5.0f;
  } else velocity.z = 0.0f;

  // Lateral movement
  if (IsKeyDown(KEY_A)) {
    velocity.x = -5.5f;
  } else if (IsKeyDown(KEY_D)) {
    velocity.x = 5.5f;
  } else velocity.x = 0.0f;

  position += velocity * dt;
}