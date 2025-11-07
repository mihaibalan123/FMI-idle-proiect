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
    void add_player();
    void choose_player();
    void choose_random_t() const;
    void close();
    static bool verify_password(const players& p);
public:
    void run();

};

inline std::istream& operator>>(std::istream& is, teachers& t) {
    is >> t.last_name>> t.first_name >> t.rarity>> t.domain>> t.aura>> t.item_id>> t.health>> t.damage>> t.critical_damage;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const teachers& t) {
    os << t.last_name << " " <<  t.first_name << " " << t.rarity << " is from the domain no. " << t.domain << ", has " << t.aura << " aura, " << t.health << " health and " << t.damage << " damage with " << t.critical_damage << "% critical_damage ";
    return os;
}

#endif //OOP_MENU_H