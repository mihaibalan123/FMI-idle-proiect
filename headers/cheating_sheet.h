#ifndef OOP_CHEATING_SHEET_H
#define OOP_CHEATING_SHEET_H
#include "item.h"

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

    static std::vector<cheating_sheet *> load_cheating_sheets();
};

#endif //OOP_CHEATING_SHEET_H
