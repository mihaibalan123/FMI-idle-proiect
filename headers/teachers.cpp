#include "teachers.h"

int teachers::turns_to_defeat(float enemy_hp, float critical_chance) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = this->damage * (1.0f + this->critical_damage * critical_chance);
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}