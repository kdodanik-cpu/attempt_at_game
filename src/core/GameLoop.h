//
// Created by Krish on 3/7/2026.
//


#ifndef ATTEMPT_AT_GAME_GAMELOOP_H
#define ATTEMPT_AT_GAME_GAMELOOP_H
#include "player/Player.h"
#include <raylib.h>


namespace GameLoop {
void Update(Player& player, Camera& cam);
void Draw(const Player& player);
float GetDeltaTime();
}
#endif // ATTEMPT_AT_GAME_GAMELOOP_H
