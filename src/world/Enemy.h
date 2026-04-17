//
// Created by vsp on 4/17/26.
//

#ifndef ATTEMPT_AT_GAME_ENEMY_H
#define ATTEMPT_AT_GAME_ENEMY_H

#include <raylib.h>

struct Enemy {

    float height;
    float radius;
    Vector3 position;
    int health;

    //Getters
    bool is_alive();

};



#endif //ATTEMPT_AT_GAME_ENEMY_H
