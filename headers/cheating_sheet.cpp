#include "cheating_sheet.h"
#include "player.h"
#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

item *cheating_sheet::clone() const {
    return new cheating_sheet(*this);
}

std::vector<cheating_sheet *> cheating_sheet::load_cheating_sheets() {
    std::vector<cheating_sheet *> cheating_sheets_list;
    std::ifstream f("cheating_sheets.json");

    nlohmann::json data = nlohmann::json::parse(f);

    for (const auto &j: data) {
        cheating_sheets_list.push_back(new cheating_sheet(
            j.value("name", ""),
            j.value("description", ""),
            j.value("rarity", ""),
            j.value("price", 0.0f),
            j.value("consumable", false),
            j.value("project_boost", 0),
            j.value("success_chance", 0.0f),
            j.value("risk_damage", 0.0f)
        ));
    }
    return cheating_sheets_list;
}

bool cheating_sheet::purchase(player& p) {
    if (p.get_health() >= this->price) {
        p.set_health(p.get_health() - this->price);
        std::cout << "Paid " << this->price << " of your own life for cheating sheet" << name << "'.\n";
        return true;
    }

    std::cout << "Not enough health! Please stay alive... Cost: " << this->price << "\n";
    return false;
}

void cheating_sheet::use(player &p) {
    std::cout << "[ITEM] Using '" << this->name << "'...\n";

    if (!p.has_any_project()) {
        std::cout << "You have no active projects to boost! (use this item later)\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double roll = dis(gen);

    std::cout << "(Success Chance: " << this->success_chance * 100 << "%)\n";

    if (roll <= this->success_chance){
        std::cout << "Success! \n";
        bool boosted = p.boost_random_project(this->project_boost);
        if(!boosted) std::cout << "Failed... \n";
    } else {
        std::cout << "You were caught! \n";
        float current_hp = p.get_health();
        float new_hp = current_hp - this->risk_damage;
        if (new_hp < 0) new_hp = 0;

        p.set_health(new_hp);
        std::cout << "You took " << this->risk_damage << " damage. Current hp is " << new_hp << "\n";

    }
}
