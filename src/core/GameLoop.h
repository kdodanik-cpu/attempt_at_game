//
// Created by Krish on 3/7/2026.
//
#ifndef ATTEMPT_AT_GAME_GAMELOOP_H
#define ATTEMPT_AT_GAME_GAMELOOP_H
#include "player/Player.h"
namespace GameLoop {
void init();
void update(Player& player);
void draw(const Player& player);
}
#endif // ATTEMPT_AT_GAME_GAMELOOP_H
