#include <fstream>
#include "menu.h"
#include "player.h"
#include "teacher.h"
#include "project.h"
#include <cstdlib>
#include <random>

void menu::display_texts(int x) {
    switch (x) {
        case 1: {
            std::cout << "Welcome to FMI-Idle Game! In order to continue you must select your player "
                    "profile or add a new one.\n";
            break;
        }
        case 2: {
            std::cout << "Player list is empty. Who are you?\n";
            break;
        }
        case 3: {
            std::cout << "Required password>";
            break;
        }
        case 4: {
            std::cout << "Access granted!\n";
            break;
        }
        case 5: {
            std::cout << "Access DENIED!\n";
            break;
        }
        case 6: {
            std::cout << "You entered in Politehnica Buisness Tower. It's a strange place isn't it? "
                    "They are ready!\n";
            break;
        }
        case 7: {
            std::cout <<
                    "Here you can see the projects you fought for! Upgrade them in order to increase you currencies and later you can even reset getting -RESTANTA- and grow stronger !"
                    << "\n";
        }
        default: ;
    }
}

void menu::show_projects() const {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    players_list[curr_player].show_projects(projects_list);
}

void menu::project_upgrade() {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }

    player& current_player = players_list[curr_player];
    const std::vector<int>& project_levels = current_player.get_project_id();

    std::cout << "Buy projects : \n";
    bool ok = false;

    for (size_t i = 0; i < project_levels.size(); ++i) {
        int level = project_levels[i];
        if (level > 0 && i < projects_list.size()) {
            const project& p = projects_list[i];
            float cost = p.get_price() * static_cast<float>(level + 1);
            std::cout << i << ". " << p.get_name() << " level: " << level <<" cost upgrade: " << cost << " and cashback: " << p.get_cashback() << "\n";
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
        selected_id >= static_cast<int>(project_levels.size()) || project_levels[selected_id] == 0)
    {
        std::cout << "Invalid or unowned project id! \n";
        return;
    }
    current_player.project_upgrade(selected_id, projects_list);
}

bool menu::verify_password(const player &player) {
    std::string temp_password;
    display_texts(3);
    std::getline(std::cin, temp_password);
    if (temp_password == player.get_password()) return true;
    return false;
}

void menu::start() {

    std::ifstream f1("projects.json");
    nlohmann::json data1 = nlohmann::json::parse(f1);

    for (const auto &i_project: data1) {
        projects_list.emplace_back(i_project);
    }

    std::ifstream f("teachers.json");
    nlohmann::json data = nlohmann::json::parse(f);

        for (const auto &i_teacher: data) {
            int project_idx = i_teacher["assigned_project"];
            if (project_idx >= 0 && project_idx < static_cast<int>(projects_list.size())) {
                const project &assigned_project = projects_list[project_idx];
                teachers_list.emplace_back(i_teacher, assigned_project);
            } else {
                teachers_list.emplace_back(i_teacher, project());
            }
        }

    std::ifstream f2("players.json");
    nlohmann::json data2 = nlohmann::json::parse(f2);

    for (const auto &i_player: data2) {
        players_list.emplace_back(i_player);
    }
}

void menu::show_stats() const{
    if (curr_player != -1) {
        std::cout << "Current player is: " << players_list[curr_player] << '\n';
        std::cout << "Currency1 value is: " << players_list[curr_player].get_currency1() << "\n";
        std::cout << "Most -beatable- domain is: " << players_list[curr_player].get_conquer_domain() << "\n";
        std::cout << "\n";
    }
    else std::cout << "No current player. Please register or select.\n";
}

void menu::add_player() {
    player temp_player;
    std::cin >> temp_player;
    players_list.push_back(temp_player);
    curr_player = static_cast<int>(players_list.size() - 1);
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
        if (option == players_list.size() + 1) add_player();
        else if (option > players_list.size() + 1) option = 0;
        else {
            unsigned long long int index = option - 1;
            if (verify_password(players_list[index])) {
                curr_player = static_cast<int>(index);
                display_texts(4);
            } else {
                display_texts(5);
                option = 0;
            }
        }
    } while (!option);
}

void menu::choose_random_t() {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    display_texts(6);

    std::vector<int> teacher_indices;
    for (int i = 0; i < static_cast<int>(teachers_list.size()); ++i) {
        teacher_indices.push_back(i);
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    int available_rerolls = 4;
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
            } else if (ok == 1) {
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

    player& current_player = players_list[curr_player];
    const teacher& teacher_fought = teachers_list[aux_t_index];

    current_player.fight_teacher(teacher_fought, aux_t_index);
}

void menu::close() {
    nlohmann::json json_players_list = players_list;
    std::ofstream f("players.json");
    f << json_players_list.dump(4);
    f.close();
}

void menu::run() {
    start();
    display_texts(1);
    int option;
    do {
        if (players_list.empty()) {
            display_texts(2);
            player temp_player;
            std::cin >> temp_player;
            players_list.push_back(temp_player);
            curr_player = 0;
        }
        std::cout << "1.Select Player\n" << "2.Show current player\n" << "3.Examination room\n" << "4.Projects Information\n" << "5.Upgrade projects\n" "0.Exit\n";
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
            display_texts(6);
            choose_random_t();
        }
        if (option == 4) {
            display_texts((7));
            show_projects();
        }
        if (option == 5) {
            project_upgrade();
        }
    } while (option);
}
