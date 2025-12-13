#include "drink.h"

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
