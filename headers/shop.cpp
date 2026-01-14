#include "shop.h"
#include "exception.h"
#include "crafting_system.h"

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
        std::cout << "0. Exit Shop\n";
        std::cout << "> ";

        try {
            option = get_verified_input(0, 7);

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
