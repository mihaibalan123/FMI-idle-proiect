#include "../headers/gadget.h"
#include "../headers/player.h"
#include "../headers/book.h"
#include "../headers/drink.h"
#include "../headers/cheating_sheet.h"
#include <fstream>
#include <random>
#include <iostream>
#include <nlohmann/json.hpp>

item *gadget::clone() const {
    return new gadget(*this);
}

void gadget::use(player &p) {
    if (this->uses_count <= 0) {
        std::cout << "Gadget is empty!\n";
        return;
    }

    std::vector<book*> loaded_books = item::load_items<book>("books.json");
    std::vector<cheating_sheet*> loaded_sheets = item::load_items<cheating_sheet>("cheating_sheets.json");
    std::vector<drink*> loaded_drinks = item::load_items<drink>("drinks.json");

    std::vector<item*> item_pool;

    item_pool.insert(item_pool.end(), loaded_books.begin(), loaded_books.end());
    item_pool.insert(item_pool.end(), loaded_sheets.begin(), loaded_sheets.end());
    item_pool.insert(item_pool.end(), loaded_drinks.begin(), loaded_drinks.end());

    this->uses_count--;
    std::cout << "Gadget activated! Uses left: " << this->uses_count << "\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, item_pool.size() - 1);

    for (int i = 0; i < this->random_items; ++i) {
        size_t random_index = dis(gen);
        const item* selected_template = item_pool[random_index];
        std::unique_ptr<item> new_reward(selected_template->clone());
        std::cout << " -> You found: " << new_reward->get_name() << " (" << new_reward->get_type() << ")\n";
        p.add_item(std::move(new_reward));
    }
    for (item* it : item_pool) {
        delete it;
    }
}

void gadget::buy_gadget(player &p, const std::vector<gadget*>& gadgets_list) {
    std::vector<item*> list(gadgets_list.begin(), gadgets_list.end());
    if (item* selected = item::select_item(list, "Gadget List")) {
        selected->purchase(p);
    }
}