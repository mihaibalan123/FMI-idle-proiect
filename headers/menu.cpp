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

void menu::close() {
    if (curr_player != -1) {
        long long current_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        players_list[curr_player].set_last_login_timestamp(current_time);
    }
    player::save_players(players_list);
}

void menu::run() {
    start();
    std::cout << "Welcome to FMI-Idle Game! In order to continue you must select your player profile or add a new one.\n";
    int option;
    do {
        if (players_list.empty()) {
            std::cout << "Player list is empty. Who are you?\n";
            player temp_player;
            std::cin >> temp_player;
            players_list.push_back(temp_player);
            curr_player = 0;
            players_list[curr_player].idle_earnings(projects_list);
        }
        std::cout << "1.Select Player / Switch user\n" << "2.Show current player basic stats\n" << "3.Examination room ---> fight a teacher\n" << "4.Player Projects Information\n" << "5.Upgrade your projects\n" << "6.RESTANTA (Reset currency)\n" "0.Exit\n";
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
            default:
                std::cout << "Optiune invalida.\n";
        }
    } while (option);
}
