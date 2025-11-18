#ifndef OOP_TEACHERS_H
#define OOP_TEACHERS_H

#include <string>
#include <iostream>
#include <utility>
#include "project.h"
class teacher {
    std::string last_name, first_name;
    int rarity, domain, aura, item_id;
    float health, damage, critical_chance;
    project assigned_project;
public:
    teacher(std::string last_name,  std::string  first_name, int rarity, int domain, int aura, int item_id, float health,
        float damage, float critical_damage, const project&  assigned_project): last_name(std::move(last_name)),
        first_name(std::move(first_name)), rarity(rarity), domain(domain),aura(aura), item_id(item_id), health(health),
        damage(damage), critical_chance(critical_damage), assigned_project(assigned_project) {}
    teacher(): rarity(0), domain(0), aura(0), item_id(0), health(0), damage(0), critical_chance(0){};
    ~teacher() = default;

    teacher(const teacher& other) :
        last_name(other.last_name),
        first_name(other.first_name),
        rarity(other.rarity),
        domain(other.domain),
        aura(other.aura),
        item_id(other.item_id),
        health(other.health),
        damage(other.damage),
        critical_chance(other.critical_chance),
        assigned_project(other.assigned_project)
    {}

    teacher& operator=(const teacher& other) {
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
        critical_chance = other.critical_chance;
        assigned_project = other.assigned_project;
        return *this;
    }

    [[nodiscard]] const std::string& get_last_name() const;
    [[nodiscard]] float get_health() const;
    [[nodiscard]] float get_critical_chance() const;
    [[nodiscard]] int get_domain() const;

    static std::vector<teacher> load_teachers(const std::vector<project>& projects_list);
    [[nodiscard]] int turns_to_defeat(float enemy_hp, float t_critical_chance) const;

    friend std::istream& operator>>(std::istream& is,  teacher &t);
    friend std::ostream& operator<<(std::ostream& os, const teacher &t);
};

inline std::istream& operator>>(std::istream& is, teacher& t) {
    is >> t.last_name>> t.first_name >> t.rarity>> t.domain>> t.aura>> t.item_id>> t.health>> t.damage>> t.critical_chance;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const teacher& t) {
    os << t.last_name << " " <<  t.first_name << " " << t.rarity << " is from the domain no. " << t.domain << ", has " << t.aura << " aura, " << t.health << " health and " << t.damage << " damage with " << t.critical_chance << "% critical_damage " << "\n";
    os << t.assigned_project;
    return os;
}

#endif //OOP_TEACHERS_H