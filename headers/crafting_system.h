#ifndef OOP_CRAFTING_SYSTEM_H
#define OOP_CRAFTING_SYSTEM_H

#include "player.h"


class crafting_system {
public:
    static void start_crafting(player& p);

private:
    static void process_combination(player& p, int idx1, int idx2);

    static bool try_combine_books(player& p, item* i1, item* i2, int idx1, int idx2);

    static bool try_combine_drinks(player& p, item* i1, item* i2, int idx1, int idx2);

    static bool try_combine_sheets(player& p, item* i1, item* i2, int idx1, int idx2);

    static float calculate_crafting_price(float p1, float p2);

    static float get_random_bonus();
};

#endif //OOP_CRAFTING_SYSTEM_H