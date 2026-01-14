#include "crafting_system.h"
#include "item.h"
#include "book.h"
#include "cheating_sheet.h"
#include "drink.h"
#include "exception.h"
#include "player.h"
#include <random>


float crafting_system::get_random_bonus() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.10f, 1.20f);
    return dis(gen);
}

float crafting_system::calculate_crafting_price(float p1, float p2) {
    float min_p;
    if (p1 < p2) {
        min_p = p1;
    } else {
        min_p = p2;
    }
    return p1 + p2 + (min_p / 2.0f);
}

bool crafting_system::try_combine_books(player& p, item* i1, item* i2, int idx1, int idx2) {
    const auto* b1 = dynamic_cast<book*>(i1);
    const auto* b2 = dynamic_cast<book*>(i2);

    if (b1 && b2) {
        std::cout << "Merging books...\n";

        std::string new_name = b1->get_name() + " + " + b2->get_name();
        float bonus_mult = get_random_bonus();

        float new_dmg = (b1->get_damage_bonus() + b2->get_damage_bonus()) * bonus_mult;
        float new_price = calculate_crafting_price(b1->get_price(), b2->get_price());

        std::cout << "Damage boosted by " << ((bonus_mult - 1.0f) * 100.0f) << "%!\n";

        auto new_item = std::make_unique<book>(
            new_name, "Crafted legendary book.", "Crafted",
            new_price, false, new_dmg
        );

        p.add_item(std::move(new_item));

        if (idx1 > idx2) {
            p.remove_item_by_index_safe(idx1);
            p.remove_item_by_index_safe(idx2);
        } else {
            p.remove_item_by_index_safe(idx2);
            p.remove_item_by_index_safe(idx1);
        }

        return true;
    }
    return false;
}

bool crafting_system::try_combine_drinks(player& p, item* i1, item* i2, int idx1, int idx2) {
    const auto* d1 = dynamic_cast<drink*>(i1);
    const auto* d2 = dynamic_cast<drink*>(i2);

    if (d1 && d2) {
        std::cout << "Mixing drinks...\n";

        std::string new_name = d1->get_name() + " + " + d2->get_name();
        float bonus_mult = get_random_bonus();

        float new_heal = (d1->get_health_restore() + d2->get_health_restore()) * bonus_mult;
        float new_price = calculate_crafting_price(d1->get_price(), d2->get_price());

        std::cout << "Health boost increased by " << ((bonus_mult - 1.0f) * 100.0f) << "%!\n";

        auto new_item = std::make_unique<drink>(
            new_name, "Crafted super potion.", "Crafted",
            new_price, true, new_heal
        );

        p.add_item(std::move(new_item));

        if (idx1 > idx2) {
            p.remove_item_by_index_safe(idx1);
            p.remove_item_by_index_safe(idx2);
        } else {
            p.remove_item_by_index_safe(idx2);
            p.remove_item_by_index_safe(idx1);
        }

        return true;
    }
    return false;
}

bool crafting_system::try_combine_sheets(player& p, item* i1, item* i2, int idx1, int idx2) {
    const auto* s1 = dynamic_cast<cheating_sheet*>(i1);
    const auto* s2 = dynamic_cast<cheating_sheet*>(i2);

    if (s1 && s2) {
        std::cout << "Combining sheets...\n";

        std::string new_name = "Master Sheet";
        float new_price = calculate_crafting_price(s1->get_price(), s2->get_price());

        int new_boost = s1->get_project_boost() + s2->get_project_boost();
        float new_risk = (s1->get_risk_damage() + s2->get_risk_damage()) / 2.0f;
        float new_chance = std::min(0.99f, (s1->get_success_chance() + s2->get_success_chance()) * 0.6f);

        auto new_item = std::make_unique<cheating_sheet>(
            new_name, "Crafted master sheet.", "Illegal",
            new_price, true, new_boost, new_chance, new_risk
        );

        p.add_item(std::move(new_item));

        if (idx1 > idx2) {
            p.remove_item_by_index_safe(idx1);
            p.remove_item_by_index_safe(idx2);
        } else {
            p.remove_item_by_index_safe(idx2);
            p.remove_item_by_index_safe(idx1);
        }

        return true;
    }
    return false;
}

void crafting_system::process_combination(player& p, int idx1, int idx2) {
    item* i1 = p.get_item_at_index_raw(idx1);
    item* i2 = p.get_item_at_index_raw(idx2);

    if (!i1 || !i2) {
        std::cout << "Error accessing items.\n";
        return;
    }

    if (try_combine_books(p, i1, i2, idx1, idx2)) return;
    if (try_combine_drinks(p, i1, i2, idx1, idx2)) return;
    if (try_combine_sheets(p, i1, i2, idx1, idx2)) return;

    std::cout << "Failed! Items must be of the same type in order to combine.\n";
}

void crafting_system::start_crafting(player& p) {
    while (true) {
        std::cout << "\n--- Crafting ---\n";
        std::cout << "Combine items to sum their stats + get 10-20% Bonus.\n";

        if (p.get_inventory_size_int() < 2) {
            std::cout << "Not enough items to craft (need at least 2).\n";
            return;
        }

        p.show_inventory_simple();
        std::cout << "0. Exit Crafting\n";

        int idx1;
        int idx2;
        int max_inv = p.get_inventory_size_int();

        std::cout << "Select first item index: ";
        try {
            idx1 = get_verified_input(0, max_inv);
        } catch (const invalid_input_error& e) {
            std::cout << e.what() << "\n";
            continue;
        } catch (const std::exception& e) {
             std::cout << "Error: " << e.what() << "\n";
             continue;
        }

        if (idx1 == 0) return;

        std::cout << "Select second item index: ";
        try {
            idx2 = get_verified_input(0, max_inv);
        } catch (const invalid_input_error& e) {
            std::cout << e.what() << "\n";
            continue;
        } catch (const std::exception& e) {
             std::cout << "Error: " << e.what() << "\n";
             continue;
        }

        if (idx2 == 0) return;

        if (idx1 == idx2) {
            std::cout << "Cannot combine an item with itself!\n";
            continue;
        }

        process_combination(p, idx1 - 1, idx2 - 1);
        std::cout << "Crafting complete.\n";
    }
}