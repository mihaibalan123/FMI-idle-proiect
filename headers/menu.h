#ifndef OOP_MENU_H
#define OOP_MENU_H
#include <vector>
#include "teacher.h"
#include "project.h"
#include "player.h"
#include "shop.h"

class menu {
    std::vector<teacher> teachers_list;
    std::vector<project> projects_list;
    std::vector<player> players_list;
    shop item_shop;
    int curr_player = -1;
    void start();
    void choose_player();
    void close();
public:
    void run();

};

#endif //OOP_MENU_H