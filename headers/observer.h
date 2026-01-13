#ifndef OOP_OBSERVER_H
#define OOP_OBSERVER_H

#include <string>

enum class Event {
    CURRENCY_GAINED,
    ENEMY_DEFEATED,
    ITEM_BOUGHT,
    PROJECT_UPGRADED
};

class EventConverter {
public:
    static std::string to_string(Event e);
    static Event from_string(const std::string& s);
};

class observer {
public:
    virtual void on_notify(Event event, int value) = 0;
    virtual ~observer() = default;
};

#endif //OOP_OBSERVER_H