#ifndef OOP_BOOK_H
#define OOP_BOOK_H
#include "item.h"

class book : public item {
    float damage_bonus;

public:
    book(std::string name, std::string description, std::string rarity, float price,
         bool consumable, float damage_bonus) : item(std::move(name), std::move(description), std::move(rarity), price,
                                                     consumable),
                                                damage_bonus(damage_bonus) {
    }

    [[nodiscard]] item *clone() const override;

    static std::vector<book *> load_books();
};



#endif //OOP_BOOK_H
