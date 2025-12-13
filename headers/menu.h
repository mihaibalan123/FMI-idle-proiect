#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <vector>
#include "teacher.h"
#include "project.h"
#include "player.h"
#include "book.h"
#include "drink.h"
#include "cheating_sheet.h"

class menu {
    std::vector<teacher> teachers_list;
    std::vector<project> projects_list;
    std::vector<player> players_list;
    std::vector<book*> books_list;
    std::vector<drink*> drinks_list;
    std::vector<cheating_sheet*> cheating_sheets_list;
    int curr_player = -1;
    void start();
    void choose_player();
    void close();
public:
    void run();

};

#endif //OOP_MENU_H