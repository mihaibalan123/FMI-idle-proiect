#ifndef OOP_TEACHERS_H
#define OOP_TEACHERS_H

#include <string>
#include <iostream>
#include <utility>
#include "projects.h"
class teachers {
    std::string last_name, first_name;
    int rarity, domain, aura, item_id;
    float health, damage, critical_damage;
    projects p;
public:
    teachers(std::string last_name,  std::string  first_name, int rarity, int domain, int aura, int item_id, float health,
        float damage, float critical_damage, const projects&  assigned_project): last_name(std::move(last_name)),
        first_name(std::move(first_name)), rarity(rarity), domain(domain),aura(aura), item_id(item_id), health(health),
        damage(damage), critical_damage(critical_damage), p(assigned_project) {}
    teachers(): rarity(0), domain(0), aura(0), item_id(0), health(0), damage(0), critical_damage(0){};
    ~teachers() = default;

    teachers(const teachers& other) :
        last_name(other.last_name),
        first_name(other.first_name),
        rarity(other.rarity),
        domain(other.domain),
        aura(other.aura),
        item_id(other.item_id),
        health(other.health),
        damage(other.damage),
        critical_damage(other.critical_damage),
        p(other.p)
    {}

    teachers& operator=(const teachers& other) {
        if (this == &other) {
            return *this;
        }
        last_name = other.last_name;
        first_name = other.first_name;
        rarity = other.rarity;
        domain = other.domain;
        aura = other.aura;
        item_id = other.item_id;
        health = other.health;
        damage = other.damage;
        critical_damage = other.critical_damage;
        p = other.p;
        return *this;
    }

    [[nodiscard]] float get_health() const {
        return health;
    }

    [[nodiscard]] float get_critical_damage() const {
        return critical_damage;
    }

    [[nodiscard]] int get_domain() const {
        return domain;
    }

    [[nodiscard]] int turns_to_defeat(float enemy_hp, float critical_chance) const;

    friend std::istream& operator>>(std::istream& is,  teachers &t);
    friend std::ostream& operator<<(std::ostream& os, const teachers &t);
};

inline std::istream& operator>>(std::istream& is, teachers& t) {
    is >> t.last_name>> t.first_name >> t.rarity>> t.domain>> t.aura>> t.item_id>> t.health>> t.damage>> t.critical_damage;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const teachers& t) {
    os << t.last_name << " " <<  t.first_name << " " << t.rarity << " is from the domain no. " << t.domain << ", has " << t.aura << " aura, " << t.health << " health and " << t.damage << " damage with " << t.critical_damage << "% critical_damage " << "\n";
    os << t.p;
    return os;
}

#endif //OOP_TEACHERS_H