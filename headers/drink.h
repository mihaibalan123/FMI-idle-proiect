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

    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string get_type() const override {
        return "drink";
    }

    static std::vector<drink *> load_drinks();

    void use(player &p) override;

    static void buy_drink(player &p, const std::vector<drink*>& drinks_list);

    nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["health_restore"] = health_restore;
        return j;
    }

};


#endif //OOP_DRINK_H
