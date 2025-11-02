#include "teachers.h"

inline std::istream& operator>>(std::istream& is, teachers& t) { // supraincarcarea operatorului
    is >> t.last_name>> t.first_name>> t.rarity>> t.domain>> t.aura>> t.health>> t.damage>> t.critical_damage>> t.item_id;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const teachers& t) {
    os << t.last_name<< " "<< t.first_name<< " "<< t.rarity<< " "<< t.domain<< " " <<t.aura<< " "
    << t.health<< " "<< t.damage<< " "<< t.critical_damage<< " "<< t.item_id;
    return os;
}