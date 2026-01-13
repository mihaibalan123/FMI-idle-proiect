#ifndef OOP_TUTORING_CLASS_H
#define OOP_TUTORING_CLASS_H
#include <iostream>
#include <random>
#include "player.h"
#include "book.h"

template <typename T>
class tutoring_class {
public:
    static void give_reward(const player& p, T value) {
        (void)p;
        (void)value;
        std::cout << "[TUTOR] I don't teach this subject yet.\n";
    }
};

template <>
class tutoring_class<float> {
public:
    static void give_reward(player& p, float tuition_fee) {
        std::cout << "\n--- Lifetime Exam ---\n";

        float current_balance = p.get_currency1();
        p.set_currency1(current_balance - tuition_fee);

        std::cout << "Tutor: " << tuition_fee << " paid.\n";
        std::cout << "TASK: Manage an imaginary fund of 1000$ over 5 years.\n";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> market_dist(-0.05f, 0.25f);

        float virtual_fund = 1000.0f;

        std::cout << "\n[Simulation]\n";
        for (int year = 1; year <= 5; ++year) {

            float rate = market_dist(gen);

            virtual_fund = virtual_fund * (1.0f + rate);

            std::cout << " Year " << year << ": Rate " << (rate * 100)
                      << "% -> Fund: " << virtual_fund << "$\n";
        }

        float total_growth = (virtual_fund - 1000.0f) / 1000.0f;
        std::cout << "\nResult: Fund grew by " << (total_growth * 100.0f) << "%.\n";

        float scholarship = 0.0f;

        if (total_growth > 0.60f) {
            scholarship = tuition_fee * 3.0f;
            std::cout << "Tutor: 'Outstanding! You are a financial genius.'\n";
        }
        else if (total_growth > 0.0f) {
            scholarship = tuition_fee * 1.2f;
            std::cout << "Tutor: 'Good job. You passed the evaluation.'\n";
        }
        else {
            scholarship = 15.0f;
            std::cout << "Tutor: 'You lost virtual money. Study harder next time.'\n";
        }

        std::cout << ">>> Rewarded: " << scholarship << " currency.\n";

        p.set_currency1(p.get_currency1() + scholarship);
    }
};

template <>
class tutoring_class<int> {
public:
    static void give_reward(player& p, int energy_hours) {
        std::cout << "\n--- Coding Workshop ---\n";
        std::cout << "Tutor: Committed " << energy_hours << " hours.\n";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> percent_dist(0.0f, 100.0f);
        std::uniform_real_distribution<float> damage_dist(5.0f, 12.0f);

        int features_completed = 0;
        float health_lost = 0.0f;

        for (int i = 1; i <= energy_hours; ++i) {

            float roll = percent_dist(gen);

            if (roll < 20.0f) {
                float dmg = damage_dist(gen);
                health_lost += dmg;
                float new_hp = std::max(1.0f, p.get_health() - dmg);
                p.set_health(new_hp);
                std::cout << " [Hour " << i << "] Bug! (-" << dmg << " hp)\n";
            }
            else if (roll > 80.0f) {
                std::cout << " [Hour " << i << "] Good! (+2 levels for an random project)\n";
                if(p.boost_random_project(2)) features_completed += 2;
            }
            else {
                std::cout << " [Hour " << i << "] Coding... Feature implemented. (+1 level for an random project)\n";
                if(p.boost_random_project(1)) features_completed++;
            }
        }

        std::cout << "\n Results: \n";
        std::cout << "Total levels gained : " << features_completed << "\n";
        std::cout << "Health lost : " << health_lost << " hp\n";
    }
};

template <>
class tutoring_class<std::string> {
public:
    static void give_reward(player& p, const std::string &base_item) {
        std::cout << "\n--- Secret Santa ---\n";
        std::cout << "Tutor: Restoring  [" << base_item << "] \n";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> quality_dist(0.0f, 100.0f);
        std::uniform_real_distribution<float> stat_dist(5.0f, 15.0f);

        std::vector<std::string> quality_prefixes = {"Broken", "Dusty", "Polished", "Antique", "Legendary"};

        float roll = quality_dist(gen);
        int tier = 0;

        if (roll > 95) tier = 4;
        else if (roll > 75) tier = 3;
        else if (roll > 40) tier = 2;
        else if (roll > 15) tier = 1;
        else tier = 0;

        std::string final_name = quality_prefixes[tier] + " " + base_item;

        float base_dmg = stat_dist(gen);
        float final_damage = static_cast<float>(tier + 1) * base_dmg;
        float value = static_cast<float>(tier + 1) * 60.0f;

        for(int k=0; k<3; k++) { std::cout << "."; }
        std::cout << "\nSuccess! It is: " << final_name << "\n";

        auto artifact = std::make_unique<book>(
            final_name,
            "Restored during Secret Santa event.",
            quality_prefixes[tier],
            value,
            false,
            final_damage
        );

        p.add_item(std::move(artifact));
        std::cout << "You kept the item! (Damage bonus: " << std::fixed << std::setprecision(1) << final_damage << ")\n";
    }
};

#endif //OOP_TUTORING_CLASS_H