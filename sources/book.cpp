#include "../headers/book.h"
#include "../headers/player.h"
#include "../headers/exception.h"
#include <iostream>
#include <nlohmann/json.hpp>

item *book::clone() const {
    return new book(*this);
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

void book::buy_book(player &p, const std::vector<book*>& books_list) {
    std::vector<item*> list(books_list.begin(), books_list.end());

    if (item* selected = item::select_item(list, "Books List")) {
        selected->purchase(p);
    }
}
