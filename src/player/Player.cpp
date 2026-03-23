//
// Created by Krish on 3/7/2026.
//

#include "Player.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <raymath.h>

static const Vector3 EYE_LEVEL_OFFSET = Vector3(0.0f, 1.6f, 0.0f);
static const float SENSITIVITY = 0.01f;
static const float PITCH_LIMIT = PI / 2.0f - 0.01f;

void Player::update(float dt, const World& world) {
  ///////////////////////
  // CONTROLLER INPUT //
  //////////////////////
  Camera camera = get_camera();

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

  // Handle vertical movement, independently
  if (IsKeyPressed(KEY_SPACE)) {
    vertical_speed = 1.5f;
  }
  position.y += vertical_speed * dt;

  constexpr float TOLERANCE = 0.005f;

  // Apply gravity
  vertical_speed -= 2.5f * dt;

  float floor_height = world.get_world_height(position.x, position.z, position.y);


  if (position.y <= floor_height && vertical_speed < 0.0f) {
    vertical_speed = 0.0f;
    position.y = floor_height;
  }

  };

void Player::handle_mouse_movement() {
  Vector2 mouse_delta = GetMouseDelta();
  yaw -= mouse_delta.x * SENSITIVITY;
  pitch -= mouse_delta.y * SENSITIVITY;
  pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT ); // Working in rads
}

// Computes camera
Camera Player::get_camera() const {
  Camera camera;
  camera.up = Vector3(0.0f, 1.0f, 0.0f);
  camera.fovy = 75.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = position + EYE_LEVEL_OFFSET;
  Vector3 forward_vector = {
    cosf(pitch) * sinf(yaw),
    sinf(pitch),
    cosf(pitch) * cosf(yaw)
  };
  camera.target = camera.position + forward_vector;
  return camera;
}