#include "../headers/achievement.h"
#include <iostream>

std::string RankConverter::to_string(Rank r) {
    switch(r) {
        case Rank::BRONZE: return "BRONZE";
        case Rank::SILVER: return "SILVER";
        case Rank::GOLD: return "GOLD";
        default: return "BRONZE";
    }
}

Rank RankConverter::from_string(const std::string& s) {
    if (s == "SILVER") return Rank::SILVER;
    if (s == "GOLD") return Rank::GOLD;
    return Rank::BRONZE;
}

nlohmann::json achievement::to_json() const {
    return {
        {"name", name},
        {"description", description},
        {"rank", RankConverter::to_string(rank)},
        {"event", EventConverter::to_string(required_event)},
        {"threshold", threshold},
        {"unlocked", unlocked}
    };
}

bool achievement::check_and_unlock(Event event, int current_value) {
    if (unlocked) return false;
    if (event == required_event && current_value >= 0 && static_cast<unsigned long long>(current_value) >= threshold) {
        unlocked = true;
        print_unlock_message();
        return true;
    }
    return false;
}

void achievement::print_status() const {
    std::string status;
    if (unlocked) {
        status = "[X]";
    } else {
        status = "[ ]";
    }
    std::cout << status << " [" << RankConverter::to_string(rank) << "] "
              << name << " -- " << description
              << " (Threshold: " << threshold << ")\n";
}

void achievement::print_unlock_message() const {
    std::cout << "\n Achievement unlocked: " << name << "\n";
    std::cout << "  Rank: " << RankConverter::to_string(rank) << "\n";
    std::cout << "  >> " << description << "\n";
}