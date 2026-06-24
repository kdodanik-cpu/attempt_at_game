//
// Created by vsp on 4/17/26.
//

#include "Enemy.h"
#include <algorithm>

#include "raymath.h"
#include "player/Player.h"

bool Enemy::is_alive() const
{
    return health > 0;
}

void Enemy::take_damage(int const damage)
{
    health = std::max(health - damage, 0);
}

void Enemy::update_state(const Player& player)
{
    float distance_from_player =
        Vector3Length(Vector3Subtract(position, player.position));
    if (state == IDLE && distance_from_player < 8.0f) state = CHASING;
    if (state == CHASING && distance_from_player > 15.0f) state = IDLE;
}



void Enemy::update(float dt, const Player& player, const World& world)
{
    //------------
    // Behavior
    //------------

    update_state(player); // Should enemy chase player?

    //------------
    // Movement
    //------------
    Vector3 distance_from_player =
        Vector3Subtract(position, player.position);
    if (Vector3Length(distance_from_player) != 0)
    {
        Vector3 direction = Vector3Normalize(distance_from_player);
        float speed = 3.0f;
        velocity = Vector3Scale(direction, speed);
    } else velocity = {0};

    if (state == CHASING) position -= velocity * dt;
}
