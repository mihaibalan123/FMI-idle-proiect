#ifndef OOP_MENU_H
#define OOP_MENU_H
#include <vector>
#include "teacher.h"
#include "project.h"
#include "player.h"
#include "shop.h"
#include "achievement_system.h"

class menu {
    std::vector<project> projects_list;
    std::vector<teacher> teachers_list;
    std::vector<player> players_list;
    shop item_shop = shop();
    int curr_player = -1;
    achievement_system Asystem;

    void choose_player();

    void close();

    static menu *main_menu;

    menu()
        : projects_list(project::load_projects()),
          teachers_list(teacher::load_teachers(projects_list)),
          players_list(player::load_players()){
    }

public:
    void run();

    menu(const menu &) = delete;

    menu &operator=(const menu &) = delete;

    static menu *get_menu() {
        if (main_menu == nullptr) main_menu = new menu;
        return main_menu;
    }
};

#endif //OOP_MENU_H
