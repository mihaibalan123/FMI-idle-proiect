#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <vector>
#include "teachers.h"
#include "projects.h"
#include "players.h"

class menu {
    std::vector<teachers> teachers_list;
    std::vector<projects> projects_list;
    std::vector<players> players_list;
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
    static bool verify_password(const players& p);
public:
    void run();

};

#endif //OOP_MENU_H