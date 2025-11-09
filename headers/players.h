#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>

class players {
    std::string name, password;
    int conquer_domain;
    float currency1, currency2, health, damage;
    std::vector<int> project_id;
    std::vector<int> defeated_domains;
public:
    players(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id = {}, std::vector<int> defeated_domains = {}): name(std::move(name)),password(std::move(password)), conquer_domain(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)), defeated_domains(std::move(defeated_domains)) {};
    players() : conquer_domain(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f) {
    }

    [[nodiscard]] const std::string& get_password() const {
        return password;
    }

    [[nodiscard]] const std::vector<int>& get_project_id() const {
        return project_id;
    }

    [[nodiscard]] float get_health() const {
        return health;
    }

    void add_project_id(int id) {
        project_id.push_back(id);
    }

    void add_defeated_domain(int domain_id) {
        defeated_domains.push_back(domain_id);
    }

    void calculate_and_set_conquer_domain() {
        if (defeated_domains.empty()) {
            conquer_domain = -1;
            return;
        }
        std::map<int, int> domain_counts;
        for (int domain : defeated_domains) {
            domain_counts[domain]++;
        }
        int max_count = -1;
        int most_frequent_domain = -1;
        for (const auto& pair : domain_counts) {
            if (pair.second > max_count) {
                max_count = pair.second;
                most_frequent_domain = pair.first;
            }
        }
        this->conquer_domain = most_frequent_domain;
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