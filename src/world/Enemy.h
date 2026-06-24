//
// Created by vsp on 4/17/26.
//

#ifndef ATTEMPT_AT_GAME_ENEMY_H
#define ATTEMPT_AT_GAME_ENEMY_H

#include <raylib.h>

struct Player;
struct World;

struct Enemy {

    float height = 1.6f;
    float radius = 0.5f;
    Vector3 position;
    Vector3 velocity = {0};
    int health = 100;

    enum State {IDLE = 0, CHASING};
    State state = IDLE;

    bool is_alive() const;
    void take_damage(int damage);
    void update_state(const Player& player);
    void update(float dt, const Player& player,
        const World& world);
};


#endif //ATTEMPT_AT_GAME_ENEMY_H
