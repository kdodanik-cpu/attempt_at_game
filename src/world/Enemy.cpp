//
// Created by vsp on 4/17/26.
//

#include "Enemy.h"

#include <algorithm>

bool Enemy::is_alive() const
{
    return health > 0;
}

void Enemy::take_damage(int const damage)
{
    health = std::max(health - damage, 0);
}