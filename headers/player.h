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
#include <chrono>

class player {
    std::string username, password;
    int current_target_domain_id;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> project_levels;
    std::vector<int> defeated_domains;
    long long last_login_timestamp = 0;
public:
    player(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id,std::vector<int> project_levels, std::vector<int> defeated_domains = {}, long long last_login_timestamp  = 0): username(std::move(name)),password(std::move(password)), current_target_domain_id(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)),project_levels(std::move(project_levels)), defeated_domains(std::move(defeated_domains)), last_login_timestamp(last_login_timestamp) {};
    player() : current_target_domain_id(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f), last_login_timestamp(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) {
        project_id = {};
        project_levels = {};
    }
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
        return current_target_domain_id;
    }

    void set_currency1(float value) {
        currency1 = value;
    }

    const std::string& get_name() const;
    float get_damage() const;
    const std::vector<int>& get_defeated_domains() const;

    [[nodiscard]] const std::vector<int>& get_project_levels() const;
    [[nodiscard]] float get_currency2() const;
    void set_currency2(float value);
    [[nodiscard]] long long get_last_login_timestamp() const;
    void set_last_login_timestamp(long long timestamp);
    void reset_projects();

    friend std::istream& operator>>(std::istream& is, player &t);
    friend std::ostream& operator<<(std::ostream& os, const player& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, username, current_target_domain_id, currency1, currency2, password, health, damage, project_id, project_levels, defeated_domains, last_login_timestamp)
};

inline std::istream& operator>>(std::istream& is, player& t) {
    std::string temp_pass1, temp_pass2;
    t.currency1 = 100.0;
    t.currency2 = 0.0;
    t.current_target_domain_id = -1;
    t.health = 500.0;
    t.damage = 7.0;
    t.project_id.clear();
    std::cout << "NEW PLAYER REGISTRATION\n";
    std::cout << "Insert player name: ";
    getline(is, t.username);
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
    os << t.username;
    return os;
}

inline void to_json(nlohmann::json& j, const player& p) {
    j = nlohmann::json{
            {"name", p.get_name()},
            {"password", p.get_password()},
            {"conquer_domain", p.get_conquer_domain()},
            {"currency1", p.get_currency1()},
            {"currency2", p.get_currency2()},
            {"health", p.get_health()},
            {"damage", p.get_damage()},
            {"project_id", p.get_project_id()},
            {"project_levels", p.get_project_levels()},
            {"defeated_domains", p.get_defeated_domains()},
            {"last_login_timestamp", p.get_last_login_timestamp()}
    };
}

#endif //OOP_PLAYER_H