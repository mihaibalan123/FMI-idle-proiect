#include <fstream>
#include "menu.h"
#include "players.h"
#include "teachers.h"
#include "projects.h"
#include <cstdlib>
#include <random>

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
        case 7: {
            std::cout << "Here you can see the projects you fought for! Upgrade them in order to increase you currencies and later you can even reset getting -RESTANTA- and grow stronger !" << "\n";
        }
        default: ;
    }
} // cam redundanta dar poate vor fi texte pe care le voi repeta deci pastrez pentru structura

void menu::show_projects() const {
    if (curr_player == -1) {
        std::cout << "You must select a player!\n";
        return;
    }
    const std::vector<int>& owned_projects = players_list[curr_player].get_project_id();

    if (owned_projects.empty()) {
        std::cout << "No projects. Go in examination room!\n";
        return;
    }
    std::cout << "\nPlayer " << players_list[curr_player] << " owns " << owned_projects.size() << " projects:\n";
    for (const auto& i : owned_projects) {
        if (i >= 0 && i <static_cast<int>(projects_list.size())) {
            const projects& p = projects_list[i];
            std::cout << i << ". " << p.get_name() << "\n";
        }
    }
    std::cout<<"\n";
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
            i_player["conquer_domain"],
            i_player["currency1"],
            i_player["currency2"],
            i_player["health"],
            i_player["damage"],
            i_player["project_id"].get<std::vector<int>>()
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

void menu::choose_random_t(){

    std::vector<teachers> teacher_list_cpy = teachers_list;
    unsigned long long int size = teachers_list.size();
    int available_rerolls = 4, fight_t;
    int x = 0, ok = 0;
    players player_fought = players_list[curr_player];
    projects project_got;

    std::vector<int> teacher_indices;
    for (unsigned long long int i= 0; i < size; i++) {
        teacher_indices.push_back(x++);
    }
    std::random_device rd;
    std::mt19937 generator(rd());
    std::ranges::shuffle(teacher_indices, generator);

    while (available_rerolls >= 0) {
        int selected_teachers = 5;
        if (available_rerolls < 4) {
            std::cout << "Reroll or keep them ? \n" << "0. Keep.\n" << "1. Reroll.\n";
            std::cout << "You have " << available_rerolls << " available rerolls left!\n";
            std::cin >> ok;
            if (ok == 0) {
                std::cout << "Selection kept. Proceeding...\n";
                break;
            }
            if (ok != 1) {
                std::cout << "??? Selection kept. Proceeding...\n";
                break;
            }
        }
        std::ranges::shuffle(teacher_indices, generator);
        std::cout << "You faced:\n";
        for (int i = 0; i < selected_teachers; ++i){
            std::cout << teacher_indices[i] << ". " << teacher_list_cpy[teacher_indices[i]] << "\n";
        }
        std::cout << "\n";
        if (available_rerolls > 0) {
            available_rerolls--;
        } else if (ok == 1) {
            std::cout << "No rerolls left. Selection kept. Proceeding...\n";
            break;
        }
    }
    for (int i = 0 ; i < 5 ;i++) {
        std::cout << i <<". Teacher no. " << teacher_indices[i] << "\n";
    }
    std::cout << "Who you wanna fight with?\n";
    std::cin >> fight_t;
    int aux_t = teacher_indices[fight_t];
    teachers teacher_fought = teachers_list[aux_t];
    if (teacher_fought.turns_to_defeat(player_fought.get_health(),teacher_fought.get_critical_damage()) < player_fought.turns_to_defeat(teacher_fought.get_health())) {
        std::cout << aux_t << ". " <<"defeated! Well done! You got project no." << aux_t << "\n";
        players_list[curr_player].add_project_id(aux_t);
    } else {
        std::cout << "You died !" << "\n";
    }

}

void menu::close() {
    nlohmann::json json_players_list = players_list;
    std::ofstream f("players.json");
    f << json_players_list.dump(4);
    f.close ();
}

void menu::run() {
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
        std::cout << "1.Select Player\n" << "2.Show current player\n"<< "3.Examination room\n"<< "4.Projects Information\n" "0.Exit\n";
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
            choose_random_t();
        }
        if (option == 4) {
            display_texts((7));
            show_projects();
        }
    } while (option);
}
