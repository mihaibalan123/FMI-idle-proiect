#include "../headers/item_loader.h"
#include "../headers/book.h"
#include "../headers/drink.h"
#include "../headers/cheating_sheet.h"
#include "../headers/gadget.h"
#include <iostream>

std::unique_ptr<item> ItemLoader::create(const nlohmann::json& item_json) {
    std::string type = item_json.value("type", "unknown");

    std::string i_name = item_json.value("name", "Unknown Item");
    std::string i_desc = item_json.value("description", "No desc");
    std::string i_rarity = item_json.value("rarity", "Common");
    float i_price = item_json.value("price", 0.0f);
    bool i_consumable = item_json.value("consumable", false);

    if (type == "book") {
        float dmg = item_json.value("damage_bonus", 0.0f);
        return std::make_unique<book>(
            i_name, i_desc, i_rarity, i_price, i_consumable, dmg
        );
    }
    else if (type == "drink") {
        float hp = item_json.value("health_restore", 0.0f);
        return std::make_unique<drink>(
            i_name, i_desc, i_rarity, i_price, i_consumable, hp
        );
    }
    else if (type == "cheating_sheet") {
        int boost = item_json.value("project_boost", 0);
        float chance = item_json.value("success_chance", 0.0f);
        float risk = item_json.value("risk_damage", 0.0f);
        return std::make_unique<cheating_sheet>(
            i_name, i_desc, i_rarity, i_price, i_consumable, boost, chance, risk
        );
    }
    else if (type == "gadget") {
        int uses = item_json.value("uses_count", 1);
        int random_items = item_json.value("random_items", 1);
        return std::make_unique<gadget>(
            i_name, i_desc, i_rarity, i_price, i_consumable, uses, random_items
        );
    }
    return nullptr;
}