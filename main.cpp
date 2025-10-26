#include <iostream>
#include <vector>

#include "headers/menu.h"
#include "headers/teachers.h"
#include "headers/projects.h"

int main() {
    std::vector<teachers> teachers_list = read_teachers("headers/teachers.txt");
    std::vector<projects> projects_list = read_projects("headers/projects.txt");

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
                    i_teacher.show_teachers();
                    i_teacher.total_damage();
                }
                option_no=0;
                break;
            case 5:
                std::cout<<"Actual projects:"<<"\n";
                for (const auto& i_project : projects_list) {
                    i_project.show_projects();
                }
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
