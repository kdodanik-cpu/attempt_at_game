//
// Created by Krish on 3/7/2026.
//

#include "Player.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <raymath.h>

static constexpr Vector3 EYE_LEVEL_OFFSET = Vector3(0.0f, 1.6f, 0.0f);
static constexpr float SENSITIVITY = 0.01f;
static constexpr float PITCH_LIMIT = PI / 2.0f - 0.01f;

void Player::shoot(const World& world)
{
  Camera cam = get_camera();
  Ray ray = {cam.position, Vector3Normalize(Vector3Subtract(cam.target, cam.position))};

  std::vector <RayCollision> candidate_rays;

  for (Box box : world.Boxes)
  {
    RayCollision ray_collision =
      GetRayCollisionBox(ray
      ,BoundingBox(
        box.position - box.dimensions * 0.5f,
        box.position + box.dimensions * 0.5f));
    if (ray_collision.hit) candidate_rays.push_back(ray_collision);
  }

  auto closest =
    std::min_element(
  candidate_rays.begin(),
  candidate_rays.end(),
  [](const RayCollision& a, const RayCollision& b)
  {
    return a.distance < b.distance;
  });

if (closest != candidate_rays.end())
{
  TraceLog(LOG_INFO, "Hit at: %f %f %f",
     closest->point.x,
     closest->point.y,
     closest->point.z);

  last_hit_point = closest->point;
  hit_marker_timer = 0.5f;
}
};

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
    vertical_speed = 3.5f;
  }
  position.y += vertical_speed * dt;



  // Apply gravity
  vertical_speed -= 2.5f * dt;

  float floor_height = world.get_world_height(position.x, position.z, position.y);


  if (position.y <= floor_height && vertical_speed < 0.0f) {
    vertical_speed = 0.0f;
    position.y = floor_height;
  }

  // Shooting code
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gun.can_fire())
  {
    shoot(world);
    gun.on_fired();
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !gun.can_fire())
    TraceLog(LOG_INFO, "Can't fire yet, time until fire: %f: ", gun.fire_rate - gun.time_since_shot);

  gun.update(dt);
  hit_marker_timer = std::max(hit_marker_timer - dt, 0.0f);
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

