#include <fstream>
#include "menu.h"
#include "players.h"
#include "teachers.h"
#include "projects.h"
#include <cstdlib>

void menu::display_texts(int x){
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
        default: break;
    }
}

bool menu::verify_password(const players& player) {
    std::string temp_password;
    display_texts(3);
    std::getline(std::cin, temp_password);
    if (temp_password == player.get_password()) return true;
    return false;
}

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
            i_player["password"],
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
    curr_player = static_cast<int>(players_list.size() - 1);
}

void menu::choose_player() {
    unsigned long long int option = 0;
    do {
        std::cout << "Players List:\n";
        for (unsigned long long int i = 0; i < players_list.size(); i++) {
            std::cout << i + 1 << "." << players_list[i]<<'\n';
        }
        std::cout << players_list.size() +1 << ". Add new player.\n" << "0.Back\n";
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 0) return;
        if (option == players_list.size()+1) add_player();
        else if (option > players_list.size()+1) option = 0;
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

void menu::close() {
    nlohmann::json json_players_list = players_list;
    std::ofstream f("players.json");
    f << json_players_list.dump(4);
    f.close ();
}

void menu::run() {
    //srand(static_cast<unsigned int>(time(nullptr)));
    start();
    display_texts(1);
    int option;
    do {
        if (players_list.empty()) {
            display_texts(2);
            players temp_player;
            std::cin >> temp_player;
            players_list.push_back(temp_player);
            curr_player = 0;
        }
        std::cout << "1.Select Player\n" << "2.Show current player\n"<< "3.Examination room\n"<< "0.Exit\n";
        std::cin >> option;
        if (!option) {
            close();
            return;
        }
        if (option == 1) {
            choose_player();
        }
        if (option == 2) {
            if (curr_player != -1) std::cout << "Current player is: "<< players_list[curr_player]<<'\n';
            else std::cout << "No current player. Please register or select.\n";
        }
        if (option == 3) {
            display_texts(6);
            //choose_random_t();
            option = 0;
            close();
        }
    } while (option);
}
