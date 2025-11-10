#include "players.h"

void players::add_project_id(int id) {
    if (id >= static_cast<int>(project_id.size())) {
        project_id.resize(id + 1,0);
    }
    project_id[id]++;
}

void players::add_defeated_domain(int domain_id) {
    defeated_domains.push_back(domain_id);
}

void players::calculate_and_set_conquer_domain() {
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

int players::turns_to_defeat(float enemy_hp) const {
    if (this->damage <= 0.0f) {
        return (enemy_hp > 0) ? -1 : 0;
    }
    float average_damage = this->damage + 0.2 * this->health;
    float turns_float = enemy_hp / average_damage;
    return static_cast<int>(turns_float);
}
