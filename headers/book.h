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

    void use(player &p) override;

    static void buy_book(player &p, const std::vector<book*>& books_list);

    [[nodiscard]] nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["damage_bonus"] = damage_bonus;
        return j;
    }
protected:
    void print1(std::ostream& os) const override {
        item::print1(os);
        os << " gives bonus damage: " << damage_bonus;
    }
};


#endif //OOP_BOOK_H
