#include "shop.h"

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

    int choice;
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(books_list.size())) {
        book *selected = books_list[choice - 1];

        if (selected->purchase(p)) {
            p.add_item(std::unique_ptr<item>(selected->clone()));
        }
    }
}

void shop::buy_drink(player &p) const {
    std::cout << "\nDrinks list: \n";
    for (size_t i = 0; i < drinks_list.size(); ++i) {
        std::cout << i + 1 << ". " << drinks_list[i]->get_name()
                << " | Price: " << drinks_list[i]->get_price()
                << " | " << drinks_list[i]->get_description() << "\n";
    }
    std::cout << "0. Back\nSelect: ";
    int choice;
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(drinks_list.size())) {
        drink *selected = drinks_list[choice - 1];
        if (selected->purchase(p)) {
            p.add_item(std::unique_ptr<item>(selected->clone()));
        }
    }
}

void shop::buy_cheating_sheet(player &p) const {
    std::cout << "\nCheating sheet list: \n";
    for (size_t i = 0; i < cheating_sheets_list.size(); ++i) {
        std::cout << i + 1 << ". " << cheating_sheets_list[i]->get_name()
                << " | Price: " << cheating_sheets_list[i]->get_price()
                << " | " << cheating_sheets_list[i]->get_description() << "\n";
    }
    std::cout << "0. Back\nSelect: ";
    int choice;
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(cheating_sheets_list.size())) {
        cheating_sheet *selected = cheating_sheets_list[choice - 1];
        if (selected->purchase(p)) {
            p.add_item(std::unique_ptr<item>(selected->clone()));
        } else {
            std::cout << "Not enough money!\n";
        }
    }
}

void shop::run(player &p) const {
    int option = 0;
    do {
        std::cout << "\nPBT Shop--PBT Shop--PBT Shop\n";
        std::cout << "Wallet: " << p.get_currency1() << "\n";
        std::cout << "1. Buy Books\n";
        std::cout << "2. Buy Drinks\n";
        std::cout << "3. Buy Cheating Sheets\n";
        std::cout << "4. Check Inventory\n";
        std::cout << "0. Exit Shop\n";
        std::cout << "> ";
        std::cin >> option;

        switch (option) {
            case 1:
                buy_book(p);
                break;
            case 2: buy_drink(p);
                break;
            case 3: buy_cheating_sheet(p);
                break;
            case 4: p.show_inventory();
                break;
            case 0: std::cout << "Leaving shop...\n";
                break;
            default: std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}
