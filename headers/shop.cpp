#include "shop.h"
#include "exception.h"
#include "crafting_system.h"

void shop::enter_gas_station(player &p) {
    std::cout << "\n=== PBT Gas Station (Currency 2 Store) ===\n";
    std::cout << "Here you spend your currency2.\n";

    while (true) {
        std::cout << "\nCurrency2 : " << p.get_currency2() << "\n";
        std::cout << "1. Wash a car (+5 Base Damage) [Cost: 2 curr2]\n";
        std::cout << "2. Sell at the counter (+50 Max Health) [Cost: 3 curr2]\n";
        std::cout << "3. Fill up some cars (Exchange 1 curr2 -> 500 curr1) [Cost: 1 curr2]\n";
        std::cout << "0. Back to normal shop\n";
        std::cout << "> ";

        int choice;
        try {
            choice = get_verified_input(0, 3);
        } catch (const std::exception& e) {
            std::cout << e.what() << "\n";
            continue;
        }

        if (choice == 0) break;

        float current_c2 = p.get_currency2();

        if (choice == 1) {
            if (current_c2 >= 2.0f) {
                p.set_currency2(current_c2 - 2.0f);
                p.set_damage(p.get_damage() + 5.0f);
                std::cout << "Damage increased by 5.\n";
            } else {
                std::cout << "Not enough currency2!\n";
            }
        }
        else if (choice == 2) {
            if (current_c2 >= 3.0f) {
                p.set_currency2(current_c2 - 3.0f);
                p.set_health(p.get_health() + 50.0f);
                std::cout << "Health increased by 50.\n";
            } else {
                std::cout << "Not enough currency2!\n";
            }
        }
        else if (choice == 3) {
            if (current_c2 >= 1.0f) {
                std::cout << "How many tokens to exchange? ";
                try {
                    int amount = get_verified_input(1, static_cast<int>(current_c2));

                    p.set_currency2(current_c2 - static_cast<float>(amount));
                    p.set_currency1(p.get_currency1() + (static_cast<float>(amount) * 500.0f));

                    std::cout << "Exchanged " << amount << " currency2 for " << (amount * 500) << " currency1.\n";
                } catch (...) {
                    std::cout << "Invalid amount.\n";
                }
            } else {
                std::cout << "You need at least 1 currency2 token.\n";
            }
        }
    }
}

void shop::sell_item(player &p) {
    if (p.get_inventory_size() == 0) {
        std::cout << "\nYour inventory is empty.\n";
        return;
    }

    std::cout << "\nSell Items\n";
    for (size_t i = 0; i < p.get_inventory_size(); ++i) {
        const item *current_item = p.get_item_at(i);
        float sell_price = current_item->get_price() / 2.0f;

        std::cout << i + 1 << ". " << current_item->get_name() << " | Sell Price: " << sell_price << "\n";
    }
    std::cout << "0. Back \n Item to sell: ";

    int choice = get_verified_input(0, static_cast<int>(p.get_inventory_size()));
    if (choice == 0) return;

    int index = choice - 1;
    const item *selected = p.get_item_at(index);
    float sell_value = selected->get_price() / 2.0f;

    std::cout << "Sold " << selected->get_name() << " for " << sell_value << " currency1.\n";

    p.set_currency1(p.get_currency1() + sell_value);
    p.remove_item_at(index);
}

void shop::run(player &p) const {
    int option = -1;

    do {
        std::cout << "\n -- PBT Shop -- PBT Shop -- PBT Shop \n";
        std::cout << "Wallet: " << p.get_currency1() << "\n";
        std::cout << "1. Buy Books\n";
        std::cout << "2. Buy Drinks\n";
        std::cout << "3. Buy Cheating Sheets\n";
        std::cout << "4. Buy Gadgets\n";
        std::cout << "5. Check Inventory\n";
        std::cout << "6. Sell an item\n";
        std::cout << "7. Crafting table\n";
        std::cout << "8. Enter in gas station\n";
        std::cout << "0. Exit Shop\n";
        std::cout << "> ";

        try {
            option = get_verified_input(0, 8);

            switch (option) {
                case 1:
                    book::buy_book(p, books_list);
                    break;
                case 2:
                    drink::buy_drink(p, drinks_list);
                    break;
                case 3:
                    cheating_sheet::buy_cheating_sheet(p, cheating_sheets_list);
                    break;
                case 4:
                    gadget::buy_gadget(p, gadgets_list);
                    break;
                case 5:
                    p.show_inventory();
                    break;
                case 6:
                    sell_item(p);
                    break;
                case 7:
                    crafting_system::start_crafting(p);
                    break;
                case 8:
                    enter_gas_station(p);
                    break;
                case 0:
                    std::cout << "Leaving shop...\n";
                    break;
                default:
                    break;
            }
        } catch (const invalid_input_error &e) {
            std::cerr << "\n" << e.what() << "\n";
        } catch (const insufficient_funds_error &e) {
            std::cerr << "\n" << e.what() << "\n";
        } catch (const std::exception &e) {
            std::cerr << "\n" << e.what() << "\n";
        }
    } while (option != 0);
}
