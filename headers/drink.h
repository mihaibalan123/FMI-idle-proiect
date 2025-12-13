#ifndef OOP_DRINK_H
#define OOP_DRINK_H
#include "item.h"

class drink : public item {
    float health_restore;

public:
    drink(std::string name, std::string description, std::string rarity, float price,
          bool consumable, float health_restore) : item(std::move(name), std::move(description), std::move(rarity), price,
                                                      consumable),
                                                 health_restore(health_restore) {
    }

    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string getType() const override {
        return "drink";
    }

    static std::vector<drink *> load_drinks();
};


#endif //OOP_DRINK_H
