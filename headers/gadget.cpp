#include "gadget.h"
#include "player.h"
#include "exception.h"
#include "book.h"
#include "drink.h"
#include "cheating_sheet.h"
#include <fstream>
#include <random>
#include <iostream>
#include <nlohmann/json.hpp>

item *gadget::clone() const {
    return new gadget(*this);
}

std::vector<gadget *> gadget::load_gadgets() {
    std::vector<gadget *> gadgets_list;
    nlohmann::json data = load_json_verified("gadgets.json");

    for (const auto &j: data) {
        gadgets_list.push_back(new gadget(
            j.value("name", "Unknown Gadget"),
            j.value("description", "No desc"),
            j.value("rarity", "Common"),
            j.value("price", 0.0f),
            j.value("consumable", false),
            j.value("uses_count", 1),
            j.value("random_items_count", 1)
        ));
    }
    return gadgets_list;
}

void gadget::use(player &p) {
    if (this->uses_count <= 0) {
        std::cout << "Gadget is empty!\n";
        return;
    }

    std::vector<book*> loaded_books = book::load_books();
    std::vector<cheating_sheet*> loaded_sheets = cheating_sheet::load_cheating_sheets();
    std::vector<drink*> loaded_drinks = drink::load_drinks();

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