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

    explicit cheating_sheet(const nlohmann::json& j) : item(j) {
        project_boost = j.value("project_boost", 0);
        success_chance = j.value("success_chance", 0.0f);
        risk_damage = j.value("risk_damage", 0.0f);
    }

    [[nodiscard]] int get_project_boost() const;

    [[nodiscard]] float get_success_chance() const;

    [[nodiscard]] float get_risk_damage() const;

    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string get_type() const override {
        return "cheating_sheet";
    }

    void use(player &p) override;

    static void buy_cheating_sheet(player &p, const std::vector<cheating_sheet*>& cheating_sheets_list);

    nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["project_boost"] = project_boost;
        j["success_chance"] = success_chance;
        j["risk_damage"] = risk_damage;
        return j;
    }
protected:
    void print1(std::ostream& os) const override {
        item::print1(os);
        os << " boosts " << project_boost
           << " project with a success chance of " << success_chance << "%"
           << " and you risk taking " << risk_damage << " damage. ";
    }
};

#endif //OOP_CHEATING_SHEET_H
