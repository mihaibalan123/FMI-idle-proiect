#ifndef OOP_CHEATING_SHEET_H
#define OOP_CHEATING_SHEET_H
#include "item.h"
#include "player.h"

class player;

class cheating_sheet : public item {
    int project_boost;
    float success_chance;
    float risk_damage;

public:
    cheating_sheet(std::string name, std::string description, std::string rarity, float price,
                   bool consumable, int project_boost, float succes_chance, float risk_damage) : item(std::move(name),
            std::move(description), std::move(rarity), price, consumable),
        project_boost(project_boost), success_chance(succes_chance), risk_damage(risk_damage) {
    }

    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string get_type() const override {
        return "cheating_sheet";
    }

    static std::vector<cheating_sheet *> load_cheating_sheets();

    void use(player &p) override;

    static void buy_cheating_sheet(player &p, const std::vector<cheating_sheet*>& cheating_sheets_list);

    nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["project_boost"] = project_boost;
        j["success_chance"] = success_chance;
        j["risk_damage"] = risk_damage;
        return j;
    }
};

#endif //OOP_CHEATING_SHEET_H
