#include "players.h"

int players::turns_to_defeat(float enemy_hp) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = this->damage + 0.2 * this->health;
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}