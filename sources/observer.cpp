#include "../headers/observer.h"

std::string EventConverter::to_string(Event e) {
    switch(e) {
        case Event::CURRENCY_GAINED: return "CURRENCY_GAINED";
        case Event::ENEMY_DEFEATED: return "ENEMY_DEFEATED";
        case Event::ITEM_BOUGHT: return "ITEM_BOUGHT";
        case Event::PROJECT_UPGRADED: return "PROJECT_UPGRADED";
        default: return "UNKNOWN";
    }
}

Event EventConverter::from_string(const std::string& s) {
    if (s == "CURRENCY_GAINED") return Event::CURRENCY_GAINED;
    if (s == "ENEMY_DEFEATED") return Event::ENEMY_DEFEATED;
    if (s == "ITEM_BOUGHT") return Event::ITEM_BOUGHT;
    return Event::PROJECT_UPGRADED;
}