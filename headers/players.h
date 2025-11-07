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
public:
    players(std::string name, std::string password, int conquer_domain, float currency1, float currency2, float health, float damage, std::vector<int> project_id = {}): name(std::move(name)),password(std::move(password)), conquer_domain(conquer_domain), currency1(currency1), currency2(currency2), health(health), damage(damage), project_id(std::move(project_id)) {};
    players() : conquer_domain(0), currency1(0.0f), currency2(0.0f), health(0.0f), damage(0.0f) {
    }

    ~players() = default;

    players (const players& other) = default;

    players& operator=(const players& other) {
        if (this == &other) return *this;
        this->name = other.name;
        this->password = other.password;
        this->conquer_domain = other.conquer_domain;
        this->currency1 = other.currency1;
        this->currency2 = other.currency2;
        this->health = other.health;
        this->damage = other.damage;
        this->project_id = other.project_id;
        return *this;
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

    [[nodiscard]] int turns_to_defeat(float enemy_hp) const;

    friend std::istream& operator>>(std::istream& is, players &t);
    friend std::ostream& operator<<(std::ostream& os, const players& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(players, name, conquer_domain, currency1, currency2, password, health, damage, project_id)
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