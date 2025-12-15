#ifndef OOP_ITEM_H
#define OOP_ITEM_H
#include <fstream>
#include <utility>
#include <vector>

class player;

class item {
protected:
    std::string name;
    std::string description, rarity;
    float price;
    bool consumable;

public:
    item(std::string name, std::string description, std::string rarity, float price,
         bool consumable) : name(std::move(name)), description(std::move(description)), rarity(std::move(rarity)),
                            price(price), consumable(consumable) {
    }

    virtual ~item() = default;

    [[nodiscard]] virtual item *clone() const = 0;

    [[nodiscard]] virtual std::string get_type() const = 0;

    virtual bool purchase(player &p) = 0;

    virtual void use(player &p) = 0;

    [[nodiscard]] const std::string &get_rarity() const {
        return rarity;
    }

    [[nodiscard]] bool get_consumable() const {
        return consumable;
    }

    [[nodiscard]] const std::string &get_name() const {
        return name;
    }

    [[nodiscard]] float get_price() const {
        return price;
    }

    [[nodiscard]] const std::string &get_description() const {
        return description;
    }
};


#endif //OOP_ITEM_H
