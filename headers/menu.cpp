#include "menu.h"
#include "players.h"
#include "teachers.h"
#include "projects.h"

void menu::start() {
    std::ifstream f("teachers.json");
    nlohmann::json data = nlohmann::json::parse(f);

    for (const auto &i_teacher: data) {
        teachers temp_teacher(
            i_teacher["last_name"],
            i_teacher["first_name"],
            i_teacher["rarity"],
            i_teacher["domain"],
            i_teacher["aura"],
            i_teacher["item_id"],
            i_teacher["health"],
            i_teacher["damage"],
            i_teacher["critical_damage"]
        );
        teachers_list.push_back(temp_teacher);
    }

    std::ifstream f1("projects.json");
    nlohmann::json data1 = nlohmann::json::parse(f1);

    for (const auto &i_project: data1) {
        projects temp_project(
            i_project["name"],
            i_project["price"],
            i_project["cashback"]

        );
        projects_list.push_back(temp_project);
    }

    std::ifstream f2("players.json");
    nlohmann::json data2 = nlohmann::json::parse(f2);

    for (const auto &i_player: data2) {
        players temp_player(
            i_player["name"],
            i_player["currency1"],
            i_player["currency2"]

        );
        players_list.push_back(temp_player);
    }
}

void menu::add_player() {
    players temp_player;
    std::cin >> temp_player;
    players_list.push_back(temp_player);
    curr_player = players_list.size() - 1;
}


void menu::choose_player() {
    unsigned long long int option = 0;
    do {
        std::cout << "Players List:\n";
        for (unsigned long long int i = 0; i < players_list.size(); i++) {
            std::cout << i + 1 << "." << players_list[i]<<'\n';
        }
        std::cout << players_list.size() +1 << ". Add new player.\n" << "0.Back.\n";
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (option == 0) return;
        if (option == players_list.size()+1) add_player();
        else if (option > players_list.size()+1) option = 0;
        else curr_player = option-1;
    } while (!option);
}


void menu::run() {
    start();
    int option;
    do {
        if (players_list.empty()) {
            players temp_player;
            std::cin >> temp_player;
            players_list.push_back(temp_player);
            curr_player = 0;
        }
        std::cout << "1.Select Player\n" << "2.Show current player\n"<< "0.Exit\n";
        std::cin >> option;
        if (!option)
            return;
        if (option == 1) {
            choose_player();
        } else if (option == 2) std::cout << players_list[curr_player]<<'\n';
    } while (option);
}
