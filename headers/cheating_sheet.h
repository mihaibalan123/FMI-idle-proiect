#ifndef OOP_CHEATING_SHEET_H
#define OOP_CHEATING_SHEET_H
#include "item.h"
#include "player.h"

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

    [[nodiscard]] int get_project_boost() const {
        return project_boost;
    }

    [[nodiscard]] float get_success_chance() const {
        return success_chance;
    }

    [[nodiscard]] float get_risk_damage() const {
        return risk_damage;
    }

    static std::vector<cheating_sheet *> load_cheating_sheets();
    bool purchase(player &p) override;
};

#endif //OOP_CHEATING_SHEET_H
