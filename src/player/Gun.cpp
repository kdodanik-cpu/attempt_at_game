//
// Created by vsp on 4/10/26.
//

#include "Gun.h"

Gun::Gun()
{
    fire_rate = 0.5f;
    time_since_shot = fire_rate;
}

bool Gun::can_fire() const
{
    return time_since_shot >= fire_rate;
}

void Gun::on_fired()
{
    time_since_shot = 0.0f;
}

void Gun::update(float dt)
{
    time_since_shot += dt;
}