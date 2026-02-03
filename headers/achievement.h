#ifndef OOP_ACHIEVEMENT_H
#define OOP_ACHIEVEMENT_H
#include <string>
#include "observer.h"
#include "nlohmann/json.hpp"

enum class Rank {
    BRONZE,
    SILVER,
    GOLD
};

class RankConverter {
public:
    static std::string to_string(Rank r);

    static Rank from_string(const std::string &s);
};

class achievement {
    std::string name;
    std::string description;
    Rank rank;
    Event required_event;
    unsigned long long int threshold;
    bool unlocked;

    void print_unlock_message() const;

public:
    achievement(std::string name, std::string description, Rank rank, Event event, int threshold, bool unlocked = false)
        : name(std::move(name)),
          description(std::move(description)),
          rank(rank),
          required_event(event),
          threshold(threshold),
          unlocked(unlocked) {
    }

    explicit achievement(const nlohmann::json &j)
        : name(j.value("name", "Unknown")),
          description(j.value("description", "No desc")),
          rank(RankConverter::from_string(j.value("rank", "BRONZE"))),
          required_event(EventConverter::from_string(j.value("event", "CURRENCY_GAINED"))),
          threshold(j.value("threshold", 100)),
          unlocked(j.value("unlocked", false)) {
    }

    [[nodiscard]] nlohmann::json to_json() const;

    bool check_and_unlock(Event event, int current_value);

    void print_status() const;

    void reset() {
        unlocked = false;
    }
};

#endif //OOP_ACHIEVEMENT_H
