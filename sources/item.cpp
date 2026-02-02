#include "../headers/item.h"
#include "../headers/player.h"
#include "../headers/exception.h"

void item::print(int i) const {
    std::cout << i << ". " << name
            << " | Price: " << price
            << " | " << description << "\n";
}

bool item::purchase(player &p) {
    if (!consumable && p.has_item(this->name)) {
        std::cout << "\n[!] You already own '" << name << "'!\n";
        return false;
    }

    if (p.get_currency1() < this->price) {
        throw insufficient_funds_error(name);
    }

    p.set_currency1(p.get_currency1() - this->price);
    p.add_item(std::unique_ptr<item>(this->clone()));

    std::cout << "Successfully purchased " << name << "! \n";
    return true;
}

item* item::select_item(const std::vector<item*>& item_list, const std::string& type) {
    std::cout << "\n--- " << type << " ---\n";

    if (item_list.empty()) {
        std::cout << "(Empty list)\n";
        return nullptr;
    }

    for (size_t i = 0; i < item_list.size(); ++i) {
        item_list[i]->print(i + 1);
    }
    std::cout << "0. Back\nSelect: ";

    int choice = get_verified_input(0, static_cast<int>(item_list.size()));

    if (choice == 0) return nullptr;
    return item_list[choice - 1];
}