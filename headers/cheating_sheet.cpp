#include "cheating_sheet.h"
#include "player.h"

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
