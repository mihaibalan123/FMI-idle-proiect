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

void menu::show_stats() const {
    if (curr_player != -1) {
        players_list[curr_player].player_stats(std::cout);
        std::cout << "\n";
    } else {
        std::cout << "No current player. Please register or select.\n";
    }
}

void menu::choose_random_teacher() {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    std::cout << "You entered in Politehnica Buisness Tower. It's a strange place isn't it? They are ready!\n";

    std::vector<int> teacher_indices;
    for (int i = 0; i < static_cast<int>(teachers_list.size()); ++i) {
        teacher_indices.push_back(i);
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    int available_rerolls = 3;
    int fight_t = -1;

    while (available_rerolls >= 0) {
        std::ranges::shuffle(teacher_indices, generator);

        std::cout << "You faced:\n";
        for (int i = 0; i < 5 && i < static_cast<int>(teacher_indices.size()); ++i) {
            int current_teacher_id = teacher_indices[i];
            std::cout << i << ". " << teachers_list[current_teacher_id] << "\n";
        }

        if (available_rerolls > 0) {
            int ok;
            std::cout << "Reroll or keep them ? \n" << "0. Keep.\n" << "1. Reroll.\n";
            std::cout << "You have " << available_rerolls << " available rerolls left!\n";
            std::cin >> ok;

            if (ok == 0) {
                std::cout << "Selection kept. Proceeding...\n";
                break;
            }
            if (ok == 1) {
                available_rerolls--;
            } else {
                std::cout << "??? Selection kept. Proceeding...\n";
                break;
            }
        } else {
            std::cout << "No rerolls left. Selection kept. Proceeding...\n";
            break;
        }
    }

    int aux_t_index = -1;
    do {
        std::cout << "Who you wanna fight with? (0-4)\n";
        std::cin >> fight_t;
        if (fight_t >= 0 && fight_t < 5) {
            aux_t_index = teacher_indices[fight_t];
        } else {
            std::cout << "Invalid selection. Try again.\n";
        }
    } while (aux_t_index == -1);

    player &current_player = players_list[curr_player];
    const teacher &teacher_fought = teachers_list[aux_t_index];

    current_player.fight_teacher(teacher_fought, aux_t_index);
}

void menu::show_projects() const {
    std::cout << "Here you can see the projects you fought for! Upgrade them in order to increase you currencies and later you can even reset getting -RESTANTA- and grow stronger !" << "\n";
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    players_list[curr_player].player_full_stats(std::cout, projects_list);
    std::cout << "\n";
}

void menu::project_upgrade() {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }

    player &current_player = players_list[curr_player];

    const std::vector<int> &project_levels = current_player.get_project_id();
    std::cout << "Buy projects : \n";
    bool ok = false;

    for (size_t i = 0; i < project_levels.size(); ++i) {
        int level = project_levels[i];
        if (level > 0 && i < projects_list.size()) {
            const project &p = projects_list[i];
            float cost = p.get_price() * static_cast<float>(level + 1);
            std::cout << i << ". " << p.get_name() << " level: " << level << " cost upgrade: " << cost << " and cashback: " << p.get_cashback() << "\n";
            ok = true;
        }
    }

    if (!ok) {
        std::cout << "Fight with a teacher first!\n";
        return;
    }

    int selected_id;
    std::cout << "\n Which project will you upgrade?? ";
    std::cin >> selected_id;

    if (selected_id < 0 || selected_id >= static_cast<int>(projects_list.size()) ||
        selected_id >= static_cast<int>(project_levels.size()) || project_levels[selected_id] == 0) {
        std::cout << "Invalid or unowned project id! \n";
        return;
    }
    current_player.project_upgrade(selected_id, projects_list);
}

void menu::reset_game() {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    player &current_player = players_list[curr_player];
    current_player.reset_game();
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
        }
        if (option == 2) {
            show_stats();
        }
        if (option == 3) {
            choose_random_teacher();
        }
        if (option == 4) {
            show_projects();
        }
        if (option == 5) {
            project_upgrade();
        }
        if (option == 6) {
            reset_game();
        }
    } while (option);
}
