#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "headers/menu.h"
#include "headers/teachers.h"
#include "headers/projects.h"

int main() {

    std::ifstream f("teachers.json");
    nlohmann::json data = nlohmann::json::parse(f);
    std::vector<teachers> teachers_list;

    for (const auto& i_teacher : data) {
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

    std::vector<std::string> options ={
        "View User Profile",
        "Economy Data",
        "Training Camp",
        "Teacher Duel", //wip
        "Projects Upgrader",//also wip
        "RESTANTA Information"
    };

    menu main_menu("Welcome to FMI ---- Idle !!!", "This is the main menu of FMI-Idle. Choose an option from the list below!", options);
    int option_no;
    do {
        main_menu.show_options();
        option_no=main_menu.choose_option();

        switch (option_no) {
            case 1:
                std::cout<<"merge1";
                option_no=0;
                break;
            case 2:
                std::cout<<"merge2";
                option_no=0;
                break;
            case 3:
                std::cout<<"merge3";
                option_no=0;
                break;
            case 4:
                std::cout<<"Teachers shown for duel:"<<"\n";
                for (const auto& i_teacher : teachers_list) {
                    std::cout<<i_teacher;
                }
                option_no=0;
                break;
            case 5:
                /*
                std::cout<<"Actual projects:"<<"\n";
                for (const auto& i_project : projects_list) {
                    i_project.show_projects();
                }
                */
                option_no=0;
                break;
            case 6:
                std::cout<<"merge6";
                option_no=0;
                break;
            default:
                std::cout<< "Goodbye!";
                break;

        }
    }while (option_no);
    return 0;
}
