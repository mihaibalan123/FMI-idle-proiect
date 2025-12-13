#include "book.h"

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
