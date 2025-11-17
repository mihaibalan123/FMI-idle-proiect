#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <vector>
#include "teacher.h"
#include "project.h"
#include "player.h"

class menu {
    std::vector<teacher> teachers_list;
    std::vector<project> projects_list;
    std::vector<player> players_list;
    int curr_player = -1;
    static void display_texts(int x);
    void start();
    void show_stats() const;
    void project_upgrade();
    void add_player();
    void choose_player();
    void choose_random_t();
    void show_projects() const;
    void close();
    static bool verify_password(const player& p);
public:
    void run();

};

#endif //OOP_MENU_H