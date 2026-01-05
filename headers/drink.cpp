#include "drink.h"
#include "exception.h"
#include <iostream>
#include <nlohmann/json.hpp>

item *drink::clone() const {
    return new drink(*this);
}

std::vector<drink *> drink::load_drinks() {
    std::vector<drink *> drinks_list;
    nlohmann::json data = load_json_verified("drinks.json");
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

void drink::use(player& p) {
    float current_hp = p.get_health();
    p.set_health(current_hp + this->health_restore);

    std::cout << "[DRINK] You consumed '" << this->name << "' and restored " << this->health_restore << "hp. \n";
    std::cout << "Current Health: " << p.get_health() << "\n";
}

void drink::buy_drink(player &p, const std::vector<drink*>& drinks_list) {
    std::vector<item*> list(drinks_list.begin(), drinks_list.end());

    if (item* selected = item::select_item(list, "Drinks List")) {
        selected->purchase(p);
    }
}