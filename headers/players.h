#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>
#include "projects.h"

class players {
    std::string name, password;
    int conquer_domain;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> project_levels;
    std::vector<int> defeated_domains;
public:
    players(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id = {}, std::vector<int> defeated_domains = {}): name(std::move(name)),password(std::move(password)), conquer_domain(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)), defeated_domains(std::move(defeated_domains)) {};
    players() : conquer_domain(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f) {}

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

    void set_currency1(float value) {
        currency1 = value;
    }

    [[nodiscard]] int get_conquer_domain() const {
        return conquer_domain;
    }

    void add_project_id(int id) {
        if (id >= static_cast<int>(project_id.size())) {
            project_id.resize(id + 1,0);
        }
        project_id[id]++;
    }
    /*
    [[nodiscard]] float get_total_cashback(const std::vector<projects>& projects_list) const {
        float total_cashback = 0.0f;
        for (int id = 0; id < static_cast<int>(project_id.size()); ++id) {
            int quantity = project_id[id];
            if (quantity > 0 && id < static_cast<int>(projects_list.size())) {
                float project_cashback = projects_list[id].get_cashback();
                total_cashback += project_cashback * static_cast<float>(quantity);
            }
        }
        return total_cashback;
    }
    */

    void add_defeated_domain(int domain_id) {
        defeated_domains.push_back(domain_id);
    }

    void calculate_and_set_conquer_domain() {
        if (defeated_domains.empty()) {
            this->conquer_domain = -1;
            return;
        }
        std::ranges::sort(defeated_domains);
        int max_count = 0;
        int mfdomain = -1;
        int curr_d = defeated_domains[0];
        int curr_c = 0;
        for (int domain : defeated_domains) {
            if (domain == curr_d) {
                curr_c++;
            } else {
                if (curr_c > max_count) {
                    max_count = curr_c;
                    mfdomain = curr_d;
                }
                curr_d = domain;
                curr_c = 1;
            }
        }
        if (curr_c > max_count) {
            mfdomain = curr_d;
        }
        this->conquer_domain = mfdomain;
    }

    [[nodiscard]] int turns_to_defeat(float enemy_hp) const;
    friend std::istream& operator>>(std::istream& is, players &t);
    friend std::ostream& operator<<(std::ostream& os, const players& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(players, name, conquer_domain, currency1, currency2, password, health, damage, project_id, defeated_domains)
};

inline std::istream& operator>>(std::istream& is, players& t) {
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

inline std::ostream& operator<<(std::ostream& os, const players& t) {
    os << t.name;
    return os;
}

#endif //OOP_PLAYER_H