#ifndef OOP_DRINK_H
#define OOP_DRINK_H
#include "item.h"
#include "player.h"
#include <vector>

class drink : public item {
    float health_restore;

public:
    drink(std::string name, std::string description, std::string rarity, float price,
          bool consumable, float health_restore) : item(std::move(name), std::move(description), std::move(rarity),
                                                        price,
                                                        consumable),
                                                   health_restore(health_restore) {
    }

    explicit drink(const nlohmann::json& j) : item(j) {
        health_restore = j.value("health_restore", 0.0f);
    }

    [[nodiscard]] float get_health_restore() const;

    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string get_type() const override {
        return "drink";
    }
    void use(player &p) override;

    static void buy_drink(player &p, const std::vector<drink*>& drinks_list);

    nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["health_restore"] = health_restore;
        return j;
    }
protected:
    void print1(std::ostream& os) const override {
        item::print1(os);
        os << " restores: " << health_restore << " hp";
    }
};


#endif //OOP_DRINK_H
