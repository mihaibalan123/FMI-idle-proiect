#include "drink.h"
#include <iostream>
#include <nlohmann/json.hpp>

item *drink::clone() const {
    return new drink(*this);
}

std::vector<drink *> drink::load_drinks() {
    std::vector<drink *> drinks_list;
    std::ifstream f1("drinks.json");
    nlohmann::json data = nlohmann::json::parse(f1);
    for (const auto &j: data) {
        drinks_list.push_back(new drink(
            j.value("name", ""),
            j.value("description", ""),
            j.value("rarity", ""),
            j.value("price", 0.0f),
            j.value("consumable", false),
            j.value("health_restore", 0.0f)));
    }
    return drinks_list;
}

bool drink::purchase(player& p) {
    if (p.get_currency2() >= this->price) {
        p.set_currency2(p.get_currency2() - this->price);
        std::cout << "Paid " << this->price << " currency2 for drink '" << name << "'.\n";
        return true;
    }

    std::cout << "Not enough currency2! Cost: " << this->price << "\n";
    return false;
}

void drink::use(player& p) {
    float current_hp = p.get_health();
    p.set_health(current_hp + this->health_restore);

    std::cout << "[DRINK] You consumed '" << this->name << "' and restored " << this->health_restore << "hp. \n";
    std::cout << "Current Health: " << p.get_health() << "\n";
}