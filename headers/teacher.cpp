#include "teacher.h"

teacher::teacher(const nlohmann::json& j, const project& assigned_project) :
    last_name(j["last_name"]),
    first_name(j["first_name"]),
    rarity(j["rarity"]),
    domain(j["domain"]),
    aura(j["aura"]),
    item_id(j["item_id"]),
    health(j["health"]),
    damage(j["damage"]),
    critical_damage(j["critical_damage"]),
    p(assigned_project)
{}

int teacher::turns_to_defeat(float enemy_hp, float critical_chance) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = this->damage * (1.0f + this->critical_damage * critical_chance);
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}

