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
  Camera camera = GetCamera();

  Vector3 move = {0};
  Vector3 distance = camera.target - camera.position;
  distance.y = 0.0f;
  Vector3 forwardVector = Vector3Normalize(distance);
  Vector3 rightVector = Vector3CrossProduct(forwardVector, Vector3(0.0f, 1.0f, 0.0f));

  // Forward movement
  if (IsKeyDown(KEY_W)) {
    move += forwardVector;
  } else if (IsKeyDown(KEY_S)) {
    move -= forwardVector;
  };
  // Lateral movement
  if (IsKeyDown(KEY_A)) {
    move -= rightVector;
  } else if (IsKeyDown(KEY_D)) {
    move += rightVector;
  }

  velocity = Vector3Scale(move, 10.0f);
  position += velocity * dt;

  // Handle vertical movement, indepdently
  if (IsKeyPressed(KEY_SPACE)) {
    verticalSpeed = 10.0f;
  }
  position.y += verticalSpeed * dt;

  if (position.y > 0.0f) {
    verticalSpeed -= 5.5f * dt; // Will be computed each frame
  } else if (position.y <= 0.0f && verticalSpeed < 0.0f) {
    verticalSpeed = 0.0f;
    position.y = 0.0f;
  }

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