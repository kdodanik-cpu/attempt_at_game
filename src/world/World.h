//
// Created by Krish on 3/16/2026.
//

#ifndef ATTEMPT_AT_GAME_WORLD_H
#define ATTEMPT_AT_GAME_WORLD_H
#include <raylib.h>
#include <vector>

struct Box {
  Vector3 dimensions;
  Vector3 position;
  Color color;
};

struct World {
  std::vector<Box> Boxes;
  void init();
  void draw() const;

  float get_world_height(float x, float z, float y) const;

};
#endif // ATTEMPT_AT_GAME_WORLD_H
