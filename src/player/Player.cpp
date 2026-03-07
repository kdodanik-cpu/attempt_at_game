//
// Created by Krish on 3/7/2026.
//

#include "Player.h"

#include <algorithm>
#include <raymath.h>

static const Vector3 EYE_LEVEL_OFFSET = Vector3(0.0f, 1.6f, 0.0f);
static const float SENSITIVITY = 0.01f;
static const float PITCH_LIMIT = PI / 2.0f - 0.01f;

void Player::Update(float dt) {
  ///////////////////////
  // CONTROLLER INPUT //
  //////////////////////

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
  };


void Player::HandleMouseMovement() {
  Vector2 MouseDelta = GetMouseDelta();
  yaw -= MouseDelta.x * SENSITIVITY;
  pitch -= MouseDelta.y * SENSITIVITY;
  pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT ); // Working in rads
}

// Computes camera
Camera Player::GetCamera() const {
  Camera camera;
  camera.up = Vector3(0.0f, 1.0f, 0.0f);
  camera.fovy = 75.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = position + EYE_LEVEL_OFFSET;
  Vector3 forwardVector = {
    cosf(pitch) * sinf(yaw),
    sinf(pitch),
    cosf(pitch) * cosf(yaw)
  };
  camera.target = camera.position + forwardVector;
  return camera;
}