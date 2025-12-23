#include "shop.h"
#include "exception.h"

void shop::start() {
    books_list = book::load_books();
    drinks_list = drink::load_drinks();
    cheating_sheets_list = cheating_sheet::load_cheating_sheets();
}

void shop::buy_book(player &p) const {
    std::cout << "\nBooks list: \n";
    for (size_t i = 0; i < books_list.size(); ++i) {
        std::cout << i + 1 << ". " << books_list[i]->get_name()
                << " | Price: " << books_list[i]->get_price()
                << " | " << books_list[i]->get_description() << "\n";
    }
    std::cout << "0. Back\nSelect: ";

    int choice = get_verified_input(0, static_cast<int>(books_list.size()));
    if (choice == 0) return;

    const book *selected = books_list[choice - 1];


    if (p.has_item(selected->get_name())) {
        std::cout << "\n[!] You already own '" << selected->get_name() << "'!\n";
        std::cout << "You cannot buy duplicates.\n";
        return;
    }

    if (p.get_currency1() < selected->get_price()) {
        throw insufficient_funds_error(selected->get_name());
    }

    p.set_currency1(p.get_currency1() - selected->get_price());
    p.add_item(std::unique_ptr<item>(selected->clone()));
    std::cout << "Successfully purchased " << selected->get_name() << "!\n";
}

void shop::buy_drink(player &p) const {
    std::cout << "\nDrinks list: \n";
    for (size_t i = 0; i < drinks_list.size(); ++i) {
        std::cout << i + 1 << ". " << drinks_list[i]->get_name()
                << " | Price: " << drinks_list[i]->get_price()
                << " | " << drinks_list[i]->get_description() << "\n";
    }
    std::cout << "0. Back\nSelect: ";

    int choice = get_verified_input(0, static_cast<int>(drinks_list.size()));
    if (choice == 0) return;

    const drink *selected = drinks_list[choice - 1];
    if (p.has_item(selected->get_name())) {
        std::cout << "\n[!] You already have '" << selected->get_name() << "' in your inventory!\n";
        std::cout << "Drink it first before buying another.\n";
        return;
    }

    if (p.get_currency1() < selected->get_price()) {
        throw insufficient_funds_error(selected->get_name());
    }

    p.set_currency1(p.get_currency1() - selected->get_price());
    p.add_item(std::unique_ptr<item>(selected->clone()));
    std::cout << "Successfully purchased " << selected->get_name() << "!\n";
}

void shop::buy_cheating_sheet(player &p) const {
    std::cout << "\nCheating sheet list: \n";
    for (size_t i = 0; i < cheating_sheets_list.size(); ++i) {
        std::cout << i + 1 << ". " << cheating_sheets_list[i]->get_name()
                << " | Price: " << cheating_sheets_list[i]->get_price()
                << " | " << cheating_sheets_list[i]->get_description() << "\n";
    }
    std::cout << "0. Back\nSelect: ";

    int choice = get_verified_input(0, static_cast<int>(cheating_sheets_list.size()));
    if (choice == 0) return;

    const cheating_sheet *selected = cheating_sheets_list[choice - 1];
    if (p.has_item(selected->get_name())) {
        std::cout << "\n[!] You already have a '" << selected->get_name() << "'!\n";
        std::cout << "Use it first or sell it.\n";
        return;
    }

    if (p.get_currency1() < selected->get_price()) {
        throw insufficient_funds_error(selected->get_name());
    }

    p.set_currency1(p.get_currency1() - selected->get_price());
    p.add_item(std::unique_ptr<item>(selected->clone()));
    std::cout << "Successfully purchased " << selected->get_name() << "!\n";
}

void shop::sell_item(player &p) {
    if (p.get_inventory_size() == 0) {
        std::cout << "\nYour inventory is empty.\n";
        return;
    }

    std::cout << "\nSell Items\n";
    for (size_t i = 0; i < p.get_inventory_size(); ++i) {
        const item* current_item = p.get_item_at(i);
        float sell_price = current_item->get_price() / 2.0f;

        std::cout << i + 1 << ". " << current_item->get_name() << " | Sell Price: " << sell_price << "\n";
    }
    std::cout << "0. Back \n Item to sell: ";

    int choice = get_verified_input(0, static_cast<int>(p.get_inventory_size()));
    if (choice == 0) return;

    int index = choice - 1;
    const item* selected = p.get_item_at(index);
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
        std::cout << "4. Check Inventory\n";
        std::cout << "5. Sell an item\n";
        std::cout << "0. Exit Shop\n";
        std::cout << "> ";

        try {
            option = get_verified_input(0, 5);

            switch (option) {
                case 1:
                    buy_book(p);
                    break;
                case 2:
                    buy_drink(p);
                    break;
                case 3:
                    buy_cheating_sheet(p);
                    break;
                case 4:
                    p.show_inventory();
                    break;
                case 5:
                    sell_item(p);
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
