#include "book.h"
#include "player.h"
#include <iostream>
#include <nlohmann/json.hpp>

item *book::clone() const {
    return new book(*this);
}

std::vector<book *> book::load_books() {
    std::vector<book *> books_list;
    std::ifstream f("books.json");
    nlohmann::json data = nlohmann::json::parse(f);
    for (const auto &j: data) {
        books_list.push_back(new book(
            j.value("name", ""),
            j.value("description", ""),
            j.value("rarity", ""),
            j.value("price", 0.0f),
            j.value("consumable", false),
            j.value("damage_bonus", 0.0f)));
    }
    return books_list;
}

bool book::purchase(player &p) {
    if (p.get_currency1() >= this->price) {
        p.set_currency1(p.get_currency1() - this->price);
        std::cout << "Paid " << this->price << " currency1 for book '" << name << "'.\n";
        return true;
    }
    std::cout << "Not enough currency1! Cost: " << this->price << "\n";
    return false;
}

void book::use(player &p) {
    if (this->consumable) {
        float current_dmg = p.get_damage();
        p.set_damage(current_dmg + this->damage_bonus);

        std::cout << "[BOOK] You read '" << this->name << "'. Damage increased by " << this->damage_bonus << ".\n";
    } else {
        std::cout << "[BOOK] You own '" << this->name << "'. Keep it in your inventory to get " << this->damage_bonus <<
                " bonus damage in fights! \n";
    }
}
