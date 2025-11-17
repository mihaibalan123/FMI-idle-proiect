#include "player.h"
#include "teacher.h"
#include <iostream>
#include <algorithm>
#include <vector>

player::player(const nlohmann::json& j) :
    name(j["name"]),
    password(j["password"]),
    conquer_domain(j["conquer_domain"]),
    currency1(j["currency1"]),
    currency2(j["currency2"]),
    health(j["health"]),
    damage(j["damage"]),
    project_id(j["project_id"]),
    defeated_domains(j.value("defeated_domains", std::vector<int>{}))
{}

void player::add_project_id(int id) {
    if (id >= static_cast<int>(project_id.size())) {
        project_id.resize(id + 1,0);
    }
    project_id[id]++;
}

void player::add_defeated_domain(int domain_id) {
    defeated_domains.push_back(domain_id);
}

void player::calculate_and_set_conquer_domain() {
    if (defeated_domains.empty()) {
        this->conquer_domain = -1;
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
    this->conquer_domain = mfdomain;
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
    constexpr float CRITICAL_CHANCE_PLAYER = 0.05f;

    int player_turns = this->turns_to_defeat(opponent.get_health());
    int teacher_turns = opponent.turns_to_defeat(this->get_health(), CRITICAL_CHANCE_PLAYER);

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
