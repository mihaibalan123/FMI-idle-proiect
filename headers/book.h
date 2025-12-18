#ifndef OOP_BOOK_H
#define OOP_BOOK_H
#include "item.h"
#include "player.h"

class book : public item {
    float damage_bonus;

public:
    book(std::string name, std::string description, std::string rarity, float price,
         bool consumable, float damage_bonus) : item(std::move(name), std::move(description), std::move(rarity), price,
                                                     consumable),
                                                damage_bonus(damage_bonus) {
    }


    [[nodiscard]] item *clone() const override;

    [[nodiscard]] std::string get_type() const override {
        return "book";
    }

    [[nodiscard]] float get_damage_bonus() const {
        return damage_bonus;
    }

    static std::vector<book *> load_books();

    bool purchase(player &p) override;

    void use(player &p) override;
protected:
    void print1(std::ostream& os) const override {
        os << " - book ---> " << description << " (price: " << price << ")";
    }
};


#endif //OOP_BOOK_H
