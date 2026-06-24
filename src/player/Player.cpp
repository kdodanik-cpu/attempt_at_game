//
// Created by Krish on 3/7/2026.
//

#include "Player.h"
#include <algorithm>
#include <complex>
#include <ostream>
#include <raymath.h>
#include <unistd.h>
#include <variant>

static constexpr Vector3 EYE_LEVEL_OFFSET = Vector3(0.0f, 1.6f, 0.0f);
static constexpr float SENSITIVITY = 0.01f;
static constexpr float PITCH_LIMIT = PI / 2.0f - 0.01f;
static constexpr float GROUNDED_SCALAR = 10.0f;
static constexpr float  AIRBORNE_SCALAR = 3.3f;
struct HitCandidate {
  RayCollision ray_coll;
  std::variant<const Box*, Enemy*> possible_entity;
};

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

void Player::handle_mouse_movement() {
  Vector2 mouse_delta = GetMouseDelta();
  yaw -= mouse_delta.x * SENSITIVITY;
  pitch -= mouse_delta.y * SENSITIVITY;
  pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT ); // Working in rads
}

void Player::check_if_grounded(const World& world)
{
   /*Knowing whether the player is resting a top an object requires access to the objects it is allowed
   to rest on. Because of this, I passed a World, which contains colliders, as a read-only parameter.
   The most obvious thing to begin discarding are objects whose top surface is above the player's bottom, as
   the player cannot rest on them. The remaining candidates ought to be checked on the basis of the X and Z axes.
   Objects not aligned on BOTH axes with the player must also be discarded since the player isn't going to touch them.
   If after all these checks more than one candidate collider remains, we simply choose that which is highest on the
   Y coordinate. There should be no issues with this because due to prior exclusions those above the player have
   already been filtered out, meaning the one highest is the one closest the player's bottom.*/


  // Prevent bugs due to floating point precision errors
  const float TOLERANCE = 0.05f;

  std::vector<Box> candidate_boxes;
  for (const Box& box: world.Boxes)
  {
    float box_top_coordinate = box.position.y + box.dimensions.y * 0.5f;
    if (position.y - radius >= box_top_coordinate - TOLERANCE)
    {
      candidate_boxes.push_back(box);
    }
  }

   /* Note for self on how this works
    * remove_if doesn't remove anything perse. It reshuffles elements in the container
    * so that those that ought to be excluded are placed at the back of the container and then returns a pointer to the
    * index where the junk you don't want to retain begins, which is the first thing you feed to .erase().
    */
  candidate_boxes.erase(
    std::remove_if(
      candidate_boxes.begin(),
      candidate_boxes.end(),
      [&](const Box& box)
      {
        bool should_remove = false; // Default
        Vector2 center_distance = {
       fabs(position.x - box.position.x),
       fabs(position.z - box.position.z)};
        Vector2 halves_distance = {
       radius + box.dimensions.x * 0.5f,
       radius + box.dimensions.z * 0.5f};

        Vector2 separation = center_distance - halves_distance;
        if (separation.x > 0 || separation.y > 0) should_remove = true;
        return should_remove;
      })
      , candidate_boxes.end()
      );

  auto candidate_box = std::max_element(
  candidate_boxes.begin(), candidate_boxes.end(),
  [](const Box& a, const Box& b)
  {
    return a.position.y < b.position.y;
  });

  if (candidate_boxes.empty()) {
    is_grounded = false;}
  else if(
    position.y - radius <= candidate_box->position.y + candidate_box->dimensions.y * 0.5f &&
    position.y - radius >= candidate_box->position.y + candidate_box->dimensions.y * 0.5f - TOLERANCE)
  {
    is_grounded =  true;
  } else is_grounded = false;
}

void Player::handle_horizontal_collisions(const World& world)
{
  for (const Box& box : world.Boxes)
  {
    BoundingBox bounding_box = {
      box.position - box.dimensions * 0.5f,
      box.position + box.dimensions * 0.5f};

    // Determine closes point in the box to the sphere's center
    Vector3 closes_point = Vector3Clamp(position, bounding_box.min, bounding_box.max);
    float squared_distance = Vector3DotProduct(closes_point - position, closes_point - position);
    if (squared_distance < radius * radius)
    {

      /*
       * I think there might be a missmatch between what the player's position is considered to be in different
       * functions.
       */

      float distance = Vector3Length(closes_point - position);
      float penetration = radius - distance;
      Vector3 normal = Vector3Normalize(position - closes_point);
      position += Vector3Scale(normal, penetration);
    }

  }
}
void Player::shoot(
  const World& world,
  std::vector<Enemy>& enemies)
{
  Camera cam = get_camera();
  Ray ray = {cam.position, Vector3Normalize(Vector3Subtract(cam.target, cam.position))};

  std::vector<HitCandidate> candidate_hits;

  for (const Box& box : world.Boxes)
  {
    RayCollision ray_collision =
      GetRayCollisionBox(ray
      ,BoundingBox(
        box.position - box.dimensions * 0.5f,
        box.position + box.dimensions * 0.5f));

    if (ray_collision.hit)
    {
      const Box* box_ptr = &box;
      HitCandidate current_hit =
        {
        ray_collision,
        box_ptr
        };
      candidate_hits.push_back(current_hit);
    }
  }
  for (Enemy& enemy : enemies)
    {
      BoundingBox enemy_bounding_box =
    { // Probably not the accurate collision box for a capsule, but will do for now
        Vector3(
          enemy.position.x - enemy.radius ,
          enemy.position.y,
          enemy.position.z - enemy.radius),
        Vector3(
          enemy.position.x + enemy.radius,
          enemy.position.y + enemy.height + enemy.radius,
          enemy.position.z + enemy.radius)
      };
      RayCollision possible_enemy_collisions =  GetRayCollisionBox(
        ray,
        enemy_bounding_box
        );

      if (possible_enemy_collisions.hit)
      {
        Enemy* enemy_ptr = &enemy;
        HitCandidate hit_candidate =
          {
          possible_enemy_collisions,
          enemy_ptr
          };
        candidate_hits.push_back(hit_candidate);
      }
    }

  auto closest =
    std::min_element(
  candidate_hits.begin(),
  candidate_hits.end(),
  [](const HitCandidate& a, const HitCandidate& b)
  {
    return a.ray_coll.distance < b.ray_coll.distance;
  });

if (closest != candidate_hits.end())
{
  TraceLog(LOG_INFO, "Hit at: %f %f %f",
     closest->ray_coll.point.x,
     closest->ray_coll.point.y,
     closest->ray_coll.point.z);

  last_hit_point = closest->ray_coll.point;
  hit_marker_timer = 0.5f;

  if (
    std::holds_alternative<Enemy*>(closest->possible_entity)
    )
  {
    Enemy* e = std::get<Enemy*>(closest->possible_entity);
    e->take_damage(10);
  }
}
};

void Player::update(float dt, const World& world, std::vector<Enemy>& enemies) {
  // ------------------------------------------------------------------------
  // CONTROLLER INPUT
  // ------------------------------------------------------------------------

  // Required variables to determine move direction according to camera
  Camera camera = get_camera();
  Vector3 move = {0};
  Vector3 distance = camera.target - camera.position;
  distance.y = 0.0f; // Zeroed because even if you look up the player shouldn't move upward
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

  // Required check that'll determine movement behavior (speed, whether player is allowed to jump, etc...)
  check_if_grounded(world);

  float scalar_to_apply = is_grounded ? GROUNDED_SCALAR : AIRBORNE_SCALAR;
  velocity = Vector3Scale(move, scalar_to_apply);
  position += velocity * dt;

  // Handle vertical movement, independently
  if (IsKeyPressed(KEY_SPACE) && is_grounded) {
    vertical_speed = 3.5f;
  }
  position.y += vertical_speed * dt;

  // Apply gravity
  vertical_speed = is_grounded ? vertical_speed : vertical_speed - 2.5f * dt;

  float floor_height = world.get_world_height(position.x, position.z, position.y - radius);

  if (position.y <= floor_height && vertical_speed < 0.0f)
  {
    vertical_speed = 0.0f;
    position.y = floor_height;
  }

  // Shooting code
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gun.can_fire())
  {
    shoot(world,enemies );
    gun.on_fired();
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !gun.can_fire())
    TraceLog(LOG_INFO, "Can't fire yet, time until fire: %f: ", gun.fire_rate - gun.time_since_shot);

  gun.update(dt);
  hit_marker_timer = std::max(hit_marker_timer - dt, 0.0f);
  };