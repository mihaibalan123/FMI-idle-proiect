#ifndef OOP_ITEM_H
#define OOP_ITEM_H
#include <fstream>
#include <utility>
#include <nlohmann/json.hpp>
#include <vector>

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

    [[nodiscard]] virtual item *clone() const =0;
};


#endif //OOP_ITEM_H
