#include "../headers/drink.h"
#include "../headers/exception.h"
#include <iostream>
#include <nlohmann/json.hpp>

float drink::get_health_restore() const {
    return health_restore;
}

item *drink::clone() const {
    return new drink(*this);
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