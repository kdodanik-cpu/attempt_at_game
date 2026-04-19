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

    bool is_alive() const;
    void take_damage(int damage);
};



#endif //ATTEMPT_AT_GAME_ENEMY_H
