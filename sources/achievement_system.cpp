#include "../headers/achievement_system.h"
#include "../headers/exception.h"
#include <fstream>
#include <iostream>

void achievement_system::load_achievements() {
    try {
        nlohmann::json data = load_json_verified("achievements.json");
        achievements_list.clear();
        for (const auto& j : data) {
            achievements_list.emplace_back(j);
        }
        reset_progress();

    } catch (const std::exception& e) {
        std::cerr << "[System] Error loading rules: " << e.what() << "\n";
    }
}

void achievement_system::on_notify(Event event, int value) {
    for (auto& ach : achievements_list) {
        ach.check_and_unlock(event, value);
    }
}

void achievement_system::show_achievements() const {
    std::cout << "\n--- Achievements progress ---\n";
    for (const auto& ach : achievements_list) {
        ach.print_status();
    }
}

void achievement_system::reset_progress() {
    for (auto& ach : achievements_list) {
        ach.reset();
    }
}