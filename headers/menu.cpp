#include <fstream>
#include "menu.h"
#include "player.h"
#include "teacher.h"
#include "project.h"
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>


void menu::start() {
    projects_list = project::load_projects();
    teachers_list = teacher::load_teachers(projects_list);
    players_list = player::load_players();
    books_list = book::load_books();
    drinks_list = drink::load_drinks();
    cheating_sheets_list = cheating_sheet::load_cheating_sheets();
}

void menu::choose_player() {
    unsigned long long int option = 0;
    do {
        std::cout << "Players List:\n";
        for (unsigned long long int i = 0; i < players_list.size(); i++) {
            std::cout << i + 1 << ". " << players_list[i] << '\n';
        }
        std::cout << players_list.size() + 1 << ". Add new  player.\n" << "0.Back\n";
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 0) return;
        if (option == players_list.size() + 1) curr_player = player::add_new_player(players_list);
        else if (option > players_list.size() + 1) option = 0;
        else {
            unsigned long long int index = option - 1;
            if (players_list[index].verify_password()) {
                curr_player = static_cast<int>(index);
                std::cout << "Access granted!\n";
                player &current_player = players_list[curr_player];
                current_player.idle_earnings(projects_list);
            } else {
                std::cout << "Access denied!\n";
                option = 0;
            }
        }
    } while (!option);
}

void buy_book(player& current_player, const std::vector<book*>& list) {

    std::cout << "Books list (Permanent Bonuses): \n";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << i + 1 << ". " << list[i]->getName()
                  << " | Price: " << list[i]->getPrice()
                  << " | " << list[i]->getDescription() << "\n";
    }
    std::cout << "0. Back\n";

    int choice;
    std::cout << "Select book to buy: ";
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(list.size())) {
        book* selected = list[choice - 1];
        float cost = selected->getPrice();

        if (current_player.get_currency1() >= cost) {
            current_player.set_currency1(current_player.get_currency1() - cost);

            item* raw_ptr = selected->clone();
            std::unique_ptr<item> smart_ptr(raw_ptr);

            current_player.add_item(std::move(smart_ptr));
            std::cout << "Successfully bought book: " << selected->getName() << "!\n";
        } else {
            std::cout << "Not enough currency! You need " << cost << ".\n";
        }
    }
}

void buy_drink(player& current_player, const std::vector<drink*>& list) {

    std::cout << "Drinks list (Health Restore): \n";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << i + 1 << ". " << list[i]->getName()
                  << " | Price: " << list[i]->getPrice()
                  << " | " << list[i]->getDescription() << "\n";
    }
    std::cout << "0. Back\n";

    int choice;
    std::cout << "Select drink to buy: ";
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(list.size())) {
        drink* selected = list[choice - 1];
        float cost = selected->getPrice();

        if (current_player.get_currency1() >= cost) {
            current_player.set_currency1(current_player.get_currency1() - cost);

            item* raw_ptr = selected->clone();
            std::unique_ptr<item> smart_ptr(raw_ptr);

            current_player.add_item(std::move(smart_ptr));
            std::cout << "Successfully bought drink: " << selected->getName() << "!\n";
        } else {
            std::cout << "Not enough currency! You need " << cost << ".\n";
        }
    }
}

void buy_cheating_sheet(player& current_player, const std::vector<cheating_sheet*>& list) {

    std::cout << "Cheating sheets list (High risk, high reward):\n";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << i + 1 << ". " << list[i]->getName()
                  << " | Price: " << list[i]->getPrice()
                  << " | " << list[i]->getDescription() << "\n";
    }
    std::cout << "0. Back\n";

    int choice;
    std::cout << "Select item to buy: ";
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(list.size())) {
        cheating_sheet* selected = list[choice - 1];
        float cost = selected->getPrice();

        if (current_player.get_currency1() >= cost) {
            current_player.set_currency1(current_player.get_currency1() - cost);

            item* raw_ptr = selected->clone();
            std::unique_ptr<item> smart_ptr(raw_ptr);

            current_player.add_item(std::move(smart_ptr));
            std::cout << "Successfully bought: " << selected->getName() << "!\n";
        } else {
            std::cout << "Not enough currency! You need " << cost << ".\n";
        }
    }
}

void menu::visit_shop() {
    int shop_option = 0;
    do {
        std::cout << "\n <-PBT SHOP-> \n";
        std::cout << "Your Currency1: " << players_list[curr_player].get_currency1() << "\n";
        std::cout << "1. Books (Permanent Bonuses)\n";
        std::cout << "2. Drinks (Health Restore)\n";
        std::cout << "3. Cheating Sheets (High risk, high reward)\n";
        std::cout << "4. Show player's inventory \n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "Your choice: ";
        std::cin >> shop_option;

        switch (shop_option) {
            case 1:
                buy_book(players_list[curr_player], books_list);
                break;
            case 2:
                buy_drink(players_list[curr_player], drinks_list);
                break;
            case 3:
                buy_cheating_sheet(players_list[curr_player], cheating_sheets_list);
                break;
            case 4:
                players_list[curr_player].show_inventory();
                break;
            case 0:
                std::cout << "Exiting shop...\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (shop_option != 0);
}


void menu::close() {
    if (curr_player != -1) {
        long long current_time = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        players_list[curr_player].set_last_login_timestamp(current_time);
    }
    player::save_players(players_list);
}

void menu::run() {
    start();
    std::cout <<
            "Welcome to FMI-Idle Game! In order to continue you must select your player profile or add a new one.\n";
    int option;
    do {
        if (players_list.empty()) {
            std::cout << "Player list is empty. Who are you?\n";
            player temp_player;
            std::cin >> temp_player;
            players_list.push_back(std::move(temp_player));
            curr_player = 0;
            players_list[curr_player].idle_earnings(projects_list);
        }
        std::cout << "1.Select Player / Switch user\n" << "2.Show current player basic stats\n" <<
                "3.Examination room ---> fight a teacher\n" << "4.Player Projects Information\n" <<
                "5.Upgrade your projects\n" << "6.RESTANTA (Reset currency)\n" << "7.SHOP (Buy Items)\n" "0.Exit\n";
        std::cin >> option;
        if (!option) {
            close();
            return;
        }
        if (option == 1) {
            choose_player();
            continue;
        }
        if (curr_player == -1) {
            std::cout << "You must select a player first!\n";
            continue;
        }
        switch (option) {
            case 2:
                players_list[curr_player].show_stats();
                break;
            case 3:
                players_list[curr_player].enter_examination_room(teachers_list);
                break;
            case 4:
                players_list[curr_player].show_projects_info(projects_list);
                break;
            case 5:
                players_list[curr_player].perform_upgrade(projects_list);
                break;
            case 6:
                players_list[curr_player].reset_progress();
                break;
            case 7 :
                visit_shop();
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (option);
}


