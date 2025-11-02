#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class players {
    std::string name, password;
    float currency1, currency2;
public:
    players(std::string name, const std::string &password, float currency1, float currency2): name(std::move(name)),password(password), currency1(currency1), currency2(currency2){};
    players() : currency1(0.0f), currency2(0.0f){}

    ~players() = default;

    players (const players& other) = default;

    players& operator=(const players& other) {
        if (this == &other) return *this;
        this->name = other.name;
        this->password = other.password;
        this->currency1 = other.currency1;
        this->currency2 = other.currency2;
        return *this;
    }

    std::string get_password() const {
        return password;
    }

    friend std::istream& operator>>(std::istream& is, players &t);
    friend std::ostream& operator<<(std::ostream& os, const players& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(players, name, currency1, currency2, password)
};


inline std::istream& operator>>(std::istream& is, players& t) {
    std::string temp_pass1, temp_pass2;
    t.currency1=100.0;
    t.currency2=0.0;
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