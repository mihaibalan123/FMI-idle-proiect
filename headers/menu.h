#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <string>
#include <vector>
#include "teachers.h"
#include "projects.h"
#include "players.h"
#include "nlohmann/json.hpp"

class menu {
    std::vector<teachers> teachers_list;
    std::vector<projects> projects_list;
    std::vector<players> players_list;
    int curr_player = 0;
    void start();
    void add_player();
    void choose_player();
    void close();
public:
    void run();

};
#endif //OOP_MENU_H