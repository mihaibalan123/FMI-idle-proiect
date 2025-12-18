#include <fstream>
#include "menu.h"
#include "player.h"
#include "teacher.h"
#include "project.h"
#include "exception.h"
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>


void menu::start() {
    projects_list = project::load_projects();
    teachers_list = teacher::load_teachers(projects_list);
    players_list = player::load_players();
    item_shop.start();
}

void menu::choose_player() {
    while (true) {
        std::cout << "\n--- PLAYERS LIST ---\n";
        for (size_t i = 0; i < players_list.size(); i++) {
            std::cout << i + 1 << ". " << players_list[i] << '\n';
        }
        std::cout << players_list.size() + 1 << ". Add new player.\n";
        std::cout << "0. Back\n";
        std::cout << "> ";
        int option;
        try {
            int max_option = static_cast<int>(players_list.size() + 1);
            option = get_verified_input(0, max_option);
        }
        catch (const invalid_input_error& e) {
            std::cerr << "\n[!] " << e.what() << "\n";
            return;
        }

        if (option == 0) {
            return;
        }

        if (option == static_cast<int>(players_list.size() + 1)) {
            curr_player = player::add_new_player(players_list);

            if (curr_player != -1) {
                players_list[curr_player].idle_earnings(projects_list);
            }
            return;
        }

        int index = option - 1;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (players_list[index].verify_password()) {
            curr_player = index;
            std::cout << "Access granted! Welcome back.\n";
            players_list[curr_player].idle_earnings(projects_list);
            return;
        } else {
            std::cout << "Access denied! Wrong password.\n";
        }
    }
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

    std::cout << "Welcome to FMI-Idle Game!\n";

    if (players_list.empty()) {
        std::cout << "Player list is empty. Who are you?\n";
        player temp_player;
        std::cin >> temp_player;
        players_list.push_back(std::move(temp_player));
        curr_player = 0;
        players_list[curr_player].idle_earnings(projects_list);
    }

    int option = -1;
    do {
        std::cout << "1. Select Player / Switch user\n"
                  << "2. Visit an easy part-time job (+currency)\n"
                  << "3. Visit a *complex* part-time job (++currency)\n"
                  << "4. Show stats\n"
                  << "5. Examination room (fight a teacher)\n"
                  << "6. Projects Information\n"
                  << "7. Projects upgrade\n"
                  << "8. Shop (buy items)\n"
                  << "9. 'Restanta' (Reset Currency)\n"
                  << "0. Exit\n";
        std::cout << "> ";

        try {
            option = get_verified_input(0, 9);

            if (option == 0) {
                close();
                return;
            }

            if (option == 1) {
                choose_player();
                continue;
            }

            if (curr_player == -1) {
                std::cout << "You must select a player first! (Option 1)\n";
                continue;
            }

            switch (option) {
                case 2:
                    players_list[curr_player].start_easy_job();
                    break;
                case 3:
                    players_list[curr_player].start_complex_job();
                    break;
                case 4:
                    players_list[curr_player].show_stats();
                    break;
                case 5:
                    players_list[curr_player].enter_examination_room(teachers_list);
                    break;
                case 6:
                    players_list[curr_player].show_projects_info(projects_list);
                    break;
                case 7:
                    players_list[curr_player].perform_upgrade(projects_list);
                    break;
                case 8:
                    item_shop.run(players_list[curr_player]);
                    break;
                case 9:
                    players_list[curr_player].reset_progress();
                    break;
                default:
                    break;
            }
        }
        catch (const insufficient_funds_error& e) {
            std::cerr << "\n" << e.what() << "\n";
            std::cout << "Tip: Go to work (option 2 or 3) to make money.\n";
        }
        catch (const player_weak_error& e) {
            std::cerr << "\n" << e.what() << "\n";
            std::cout << "Tip: Buy drinks from the shop to heal.\n";
        }
        catch (const invalid_input_error& e) {
            std::cerr << "\n" << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\n" << e.what() << "\n";
        }

    } while (option != 0);
}