//
// Created by Krish on 3/16/2026.
//

#include "World.h"

#include <iostream>
#include <raymath.h>

void World::init() {
  // Ground
  Boxes.push_back(
    Box{
      Vector3{100.0f, 1.0f, 100.0f},
      Vector3 {0, -0.5f, 0 },
      LIGHTGRAY
    });
  // Box1
  Boxes.push_back(  Box{
    Vector3{1.0f, 1.0f, 1.0f},
    Vector3{2.0f, 0.5f, 4.0f},
      RED
    });
  // Box2
  Boxes.push_back(
    Box{
    Vector3{1.0f, 1.0f, 1.0f},
      Vector3{1.0f, 0.5f, 3.0f},
      GREEN
    });
}

float World::get_world_height(float x, float z, float y) const {

  std::vector<float> candidate_heights;
  candidate_heights.push_back(-900.0f); // Max fall distance
  for (Box box : Boxes) {
    Vector3 max_bounds = box.position + box.dimensions * 0.5f;
    Vector3 min_bounds = box.position - box.dimensions * 0.5f;

    if (x >= min_bounds.x && x <= max_bounds.x
      && z >= min_bounds.z && z <= max_bounds.z
      && y > min_bounds.y) candidate_heights.push_back(max_bounds.y);
  }
    return *std::max_element(candidate_heights.begin(), candidate_heights.end());

}

void World::draw() const {

  // Draw ground as a cube
  for (const Box& box : Boxes) {
    DrawCubeV(
      box.position,
      box.dimensions,
      box.color);
    DrawCubeWiresV(
    box.position,
    box.dimensions,
    BLACK
      );
  }
}