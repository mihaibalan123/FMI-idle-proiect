#ifndef OOP_ACHIEVEMENT_SYSTEM_H
#define OOP_ACHIEVEMENT_SYSTEM_H
#include "observer.h"
#include "achievement.h"
#include <vector>
#include <string>

class achievement_system : public observer {
    std::vector<achievement> achievements_list;
    void load_achievements();
public:
    achievement_system(){
        load_achievements();
    }
    ~achievement_system() override {};

    void on_notify(Event event, int value) override;

    void show_achievements() const;

    void reset_progress();
};

#endif //OOP_ACHIEVEMENT_SYSTEM_H