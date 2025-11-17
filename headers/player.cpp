#include "player.h"
#include "teacher.h"
#include <iostream>
#include <algorithm>
#include <vector>

const std::string& player::get_name() const {
    return username;
}

float player::get_damage() const {
    return damage;
}

const std::vector<int>& player::get_defeated_domains() const {
    return defeated_domains;
}

long long player::get_last_login_timestamp() const {
    return last_login_timestamp;
}

void player::set_last_login_timestamp(long long timestamp) {
    last_login_timestamp = timestamp;
}

void player::reset_projects() {
    project_id.clear();
    project_levels.clear();
}

[[nodiscard]] const std::vector<int>& player::get_project_levels() const {
    return project_levels;
}

[[nodiscard]] float player::get_currency2() const {
    return currency2;
}

void player::set_currency2(float value) {
    currency2 = value;
}

void player::add_project_id(int id) {
    if (id >= static_cast<int>(project_id.size())) {
        project_id.resize(id + 1, 0);
        project_levels.resize(id + 1, 0);
    }
    project_id[id]++;
    project_levels[id]++;
}

void player::add_defeated_domain(int domain_id) {
    defeated_domains.push_back(domain_id);
}

void player::calculate_and_set_conquer_domain() {
    if (defeated_domains.empty()) {
        this->current_target_domain_id = -1;
        return;
    }
    std::ranges::sort(defeated_domains);
    int max_count = 0;
    int mfdomain = -1;
    int curr_d = defeated_domains[0];
    int curr_c = 0;
    for (int domain : defeated_domains) {
        if (domain == curr_d) {
            curr_c++;
        } else {
            if (curr_c > max_count) {
                max_count = curr_c;
                mfdomain = curr_d;
            }
            curr_d = domain;
            curr_c = 1;
        }
    }
    if (curr_c > max_count) {
        mfdomain = curr_d;
    }
    this->current_target_domain_id = mfdomain;
}

int player::turns_to_defeat(float enemy_hp) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = damage + 0.2f * this->health;
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}

void player::show_projects(const std::vector<project>& projects_list) const {
    const std::vector<int> &owned_projects = this->get_project_id();

    if (owned_projects.empty() ) {
        std::cout << "No projects. Go in examination room!\n";
        return;
    }
    std::cout << "\nPlayer " << *this << " owns projects at these levels:\n";
    for (int i = 0; i < static_cast<int>(owned_projects.size()); ++i) {
        int level = owned_projects[i];
        if (level > 0 && i < static_cast<int>(projects_list.size())) {
            const project &p = projects_list[i];
            std::cout << i << ". " << p.get_name() << " [Level: " << level << "]" << "\n";
        }
    }
    std::cout << "\n";
}

void player::project_upgrade(int selected_id, const std::vector<project>& projects_list) {

    if (selected_id < 0 || selected_id >= static_cast<int>(projects_list.size())) {
        std::cout << "Invalid project ID for upgrade!\n";
        return;
    }

    const project& selected_project = projects_list[selected_id];
    const std::vector<int>& p_project_levels = this->get_project_id();

    if (selected_id >= static_cast<int>(p_project_levels.size()) || p_project_levels[selected_id] == 0) {
        std::cout << "You don't own project " << selected_id << ". This should be checked in menu!\n";
        return;
    }

    int current_level = p_project_levels[selected_id];
    float cost = selected_project.get_price() * static_cast<float>(current_level + 1);
    float current_currency = this->get_currency1();

    if (current_currency >= cost) {
        this->set_currency1(current_currency - cost);
        this->add_project_id(selected_id);

        float reward = selected_project.get_cashback() * static_cast<float>(current_level + 1) * 10.0f;
        this->set_currency1(this->get_currency1() + reward);

        std::cout << "\n[SUCCES] project \"" << selected_project.get_name() << "\" was upgraded to level " << current_level + 1 << "\n";
        std::cout << "New currency1: " << this->get_currency1() << "\n";
    } else {
        std::cout << "Not enough money! Required: " << cost << ", Available: " << current_currency << ".\n";
    }
}

void player::fight_teacher(const teacher& opponent, int p_project_id) {
    float critical_chance = opponent.get_critical_chance();

    int player_turns = this->turns_to_defeat(opponent.get_health());
    int teacher_turns = opponent.turns_to_defeat(this->get_health(), critical_chance);

    if (player_turns != -1 && (teacher_turns == -1 || player_turns <= teacher_turns)) {
        std::cout << opponent.get_last_name() << " defeated! Well done! You got project no." << p_project_id << "\n";

        this->add_project_id(p_project_id);
        int domain_id = opponent.get_domain();
        this->add_defeated_domain(domain_id);
        this->calculate_and_set_conquer_domain();
    } else {
        std::cout << "You died! Teacher " << opponent.get_last_name() << " was stronger." << "\n";
    }
}

void player::idle_earnings(const std::vector<project>& projects_list) {
    long long current_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    long long time_elapsed_seconds = current_time - get_last_login_timestamp();
    constexpr long long parse_seconds = 5 * 60;

    if (time_elapsed_seconds <= 0) {
        set_last_login_timestamp(current_time);
        return;
    }

    long long tick_count = time_elapsed_seconds / parse_seconds;

    if (tick_count > 0) {
        float total_idle_earnings = 0.0f;
        const std::vector<int> &i_project_levels = get_project_levels();

        for (size_t i = 0; i < i_project_levels.size() && i < projects_list.size(); ++i) {
            int level = i_project_levels[i];
            if (level > 0) {
                const project &p = projects_list[i];
                float earnings_per_tick = p.get_cashback() * static_cast<float>(level);
                total_idle_earnings += earnings_per_tick;
            }
        }

        float final_earnings = total_idle_earnings * static_cast<float>(tick_count);
        set_currency1(get_currency1() + final_earnings);

        std::cout << "\nIdle Income: You were away for " << time_elapsed_seconds / 3600 << " hours and " <<(time_elapsed_seconds % 3600) / 60 << " minutes and " << (time_elapsed_seconds % 60) << " seconds.\n";
        std::cout << "You earned " << final_earnings << " currency1!\n";
    }
    set_last_login_timestamp(current_time);
}

void player::reset_game() {
    float c1_to_reset = get_currency1();

    float multiplier = 5.0f;
    float earned_currency2 = std::log10(c1_to_reset + 1.0f) * multiplier;

    std::cout << "Do you want to reset? \n";
    std::cout << "You have " << c1_to_reset << " currency1 and will earn " << earned_currency2 << " currency2.\n";
    std::cout << "This will reset your Currency1 and all Project levels.\n";
    std::cout << "Type 'ok' to confirm the reset or anything else not to: ";

    std::string confirmation;
    std::cin >> confirmation;

    if (confirmation == "ok") {
        set_currency2(get_currency2() + earned_currency2);
        set_currency1(0.0f);
        this->reset_projects();
        std::cout << "Success!\n";
    } else {
        std::cout << "Cancelled.\n";
    }
}
