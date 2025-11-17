#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>
#include <algorithm>
#include "project.h"
#include "teacher.h"
#include <vector>

class player {
    std::string name, password;
    int conquer_domain;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> project_levels;
    std::vector<int> defeated_domains;
public:
    player(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id = {}, std::vector<int> defeated_domains = {}): name(std::move(name)),password(std::move(password)), conquer_domain(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)), defeated_domains(std::move(defeated_domains)) {};
    player() : conquer_domain(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f) {}
    explicit player(const nlohmann::json& j);
    ~player() = default;

    void add_project_id(int id);
    void add_defeated_domain(int domain_id);
    void calculate_and_set_conquer_domain();
    [[nodiscard]] int turns_to_defeat(float enemy_hp) const;

    void show_projects(const std::vector<project>& projects_list) const;
    void project_upgrade(int selected_id, const std::vector<project>& projects_list);
    void fight_teacher(const teacher& opponent, int p_project_id);

    [[nodiscard]] const std::string& get_password() const {
        return password;
    }

    [[nodiscard]] const std::vector<int>& get_project_id() const {
        return project_id;
    }

    [[nodiscard]] float get_health() const {
        return health;
    }

    [[nodiscard]] float get_currency1() const {
        return currency1;
    }

    [[nodiscard]] int get_conquer_domain() const {
        return conquer_domain;
    }

    void set_currency1(float value) {
        currency1 = value;
    }

    friend std::istream& operator>>(std::istream& is, player &t);
    friend std::ostream& operator<<(std::ostream& os, const player& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, name, conquer_domain, currency1, currency2, password, health, damage, project_id, defeated_domains)
};

inline std::istream& operator>>(std::istream& is, player& t) {
    std::string temp_pass1, temp_pass2;
    t.currency1 = 100.0;
    t.currency2 = 0.0;
    t.conquer_domain = -1;
    t.health = 500.0;
    t.damage = 7.0;
    t.project_id.clear();
    std::cout << "NEW PLAYER REGISTRATION\n";
    std::cout << "Insert player name: ";
    getline(is, t.name);
    do {
        std::cout << "Insert your password> ";
        is >> temp_pass1;
        std:: cout << "Re-type your password# ";
        is >> temp_pass2;
        if (temp_pass1 != temp_pass2) std::cout << "Retry. Passwords doesn't match!\n";
        if (temp_pass1.size() < 5) std::cout << "Password must be at least 5 chars long!\n";
    } while (temp_pass1 != temp_pass2 || temp_pass1.size()< 5);
    t.password = temp_pass1;
    return is;
}
inline std::ostream& operator<<(std::ostream& os, const player& t) {
    os << t.name;
    return os;
}

#endif //OOP_PLAYER_H