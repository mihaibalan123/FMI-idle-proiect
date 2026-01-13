#ifndef OOP_ITEM_H
#define OOP_ITEM_H
#include <fstream>
#include <utility>
#include <vector>
#include "nlohmann/json.hpp"
#include "exception.h"

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

    explicit item(const nlohmann::json& j) {
        name = j.value("name", "Unknown");
        description = j.value("description", " ");
        rarity = j.value("rarity", "Common");
        price = j.value("price", 0.0f);
        consumable = j.value("consumable", false);
    }

    template <typename T>
    static std::vector<T*> load_items(const std::string& filename) {
        std::vector<T*> items_list;
        nlohmann::json data = load_json_verified(filename);
        for (const auto& j : data) {
            items_list.push_back(new T(j));
        }
        return items_list;
    }

    virtual ~item() = default;

    void print(std::ostream& os) const {
        os << "[" << get_type() << "] " << name << " | ";
        print1(os);
    }

    friend std::ostream& operator<<(std::ostream& os, const item& it) {
        it.print(os);
        return os;
    }

    [[nodiscard]] virtual item *clone() const = 0;

    [[nodiscard]] virtual std::string get_type() const = 0;

    virtual bool purchase(player &p);

    void print(int index) const;

    static item* select_item(const std::vector<item*>& item_list, const std::string& type);

    virtual void use(player &p) = 0;

    [[nodiscard]] bool get_consumable() const {
        return consumable;
    }

    [[nodiscard]] const std::string &get_name() const {
        return name;
    }

    [[nodiscard]] float get_price() const {
        return price;
    }

    virtual nlohmann::json to_json() const {
        nlohmann::json j;
        j["type"] = get_type();
        j["name"] = name;
        j["description"] = description;
        j["price"] = price;
        j["rarity"] = rarity;
        j["consumable"] = consumable;
        return j;
    }

protected:
    virtual void print1(std::ostream& os) const {
        os << " (price: " << price << ")";
    }
};


#endif //OOP_ITEM_H
