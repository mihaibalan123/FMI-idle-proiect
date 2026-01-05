#ifndef OOP_GADGET_H
#define OOP_GADGET_H
#include "item.h"
#include "player.h"
#include <vector>

class gadget : public item {
    int uses_count;
    int random_items;

public:
    gadget(std::string name, std::string description, std::string rarity, float price,
           bool consumable, int uses_count, int random_items_count)
        : item(std::move(name), std::move(description), std::move(rarity), price, consumable),
          uses_count(uses_count),
          random_items(random_items_count) {
    }

    [[nodiscard]] item *clone() const override;

    static std::vector<gadget *> load_gadgets();

    static void buy_gadget(player &p, const std::vector<gadget*>& gadgets_list);

    [[nodiscard]] std::string get_type() const override {
        return "gadget";
    }

    void use(player &p) override;

    nlohmann::json to_json() const override {
        nlohmann::json j = item::to_json();
        j["uses_count"] = uses_count;
        j["random_items_count"] = random_items;
        return j;
    }

protected:
    void print1(std::ostream &os) const override {
        item::print1(os);
        os << " uses left " << uses_count << " and drops " << random_items << " items";
    }
};


#endif
