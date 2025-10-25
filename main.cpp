#include <iostream>
#include <vector>

#include "meniu.h"

int main() {

    std::vector<std::string> options ={
        "View User Profile",
        "Economy Data",
        "Training Camp",
        "Teacher Duel",
        "Projects Upgrader",
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
                break;
            case 2:
                std::cout<<"merge2";
                break;
            case 3:
                std::cout<<"merge3";
                break;
            case 4:
                std::cout<<"merge4";
                break;
            case 5:
                std::cout<<"merge5";
                break;
            case 6:
                std::cout<<"merge6";
                break;
            default:
                std::cout<< "Goodbye!";
                break;

        }
    }while (option_no);

    return 0;
}
