//
// Created by vsp on 4/10/26.
//

#ifndef ATTEMPT_AT_GAME_GUN_H
#define ATTEMPT_AT_GAME_GUN_H



struct Gun {
    float fire_rate; // Seconds between shots
    float time_since_shot; // accumulator

    Gun();
    bool can_fire() const;
    void on_fired();
    void update(float dt);
};



#endif //ATTEMPT_AT_GAME_GUN_H
