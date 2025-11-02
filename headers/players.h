#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include <string>
#include <nlohmann/json.hpp>

class players {
    std::string name;
    float currency1, currency2;
public:
    players(std::string name, float currency1, float currency2): name(std::move(name)), currency1(currency1), currency2(currency2){};
    players() : currency1(0.0f), currency2(0.0f){}

    ~players() = default;

    players (const players& other) = default;

    players& operator=(const players& other) {
        if (this == &other) return *this;
        this->name = other.name;
        this->currency1 = other.currency1;
        this->currency2 = other.currency2;
        return *this;
    }

    friend std::istream& operator>>(std::istream& is, players &t);
    friend std::ostream& operator<<(std::ostream& os, const players& t);
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(players, name, currency1, currency2);
};


inline std::istream& operator>>(std::istream& is, players& t) {
    t.currency1=100.0;
    t.currency2=0;
    std::cout << "NEW PLAYER REGISTRATION\n";
    std::cout << "Insert player name: ";
    getline(is, t.name);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const players& t) {
    os << t.name;
    return os;
}

#endif //OOP_PLAYER_H