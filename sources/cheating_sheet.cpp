#include "../headers/cheating_sheet.h"
#include "../headers/player.h"
#include "../headers/exception.h"
#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

int cheating_sheet::get_project_boost() const {
    return project_boost;
}

float cheating_sheet::get_success_chance() const {
    return success_chance;
}

float cheating_sheet::get_risk_damage() const {
    return risk_damage;
}

item *cheating_sheet::clone() const {
    return new cheating_sheet(*this);
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

void cheating_sheet::buy_cheating_sheet(player &p, const std::vector<cheating_sheet*>& cheating_sheet_list) {
    std::vector<item*> list(cheating_sheet_list.begin(), cheating_sheet_list.end());

    if (item* selected = item::select_item(list, "Black Market")) {
        selected->purchase(p);
    }
}
